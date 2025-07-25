﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaCharacter.h"
#include "EnhancedInputComponent.h"
#include "SpartaPlayerController.h"
#include "SpartaGameState.h"
#include "Components\Image.h"
#include "Components\ProgressBar.h"
#include "camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components\TextBlock.h"

// Sets default values
ASpartaCharacter::ASpartaCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300;
	SpringArmComp->bUsePawnControlRotation = true;


	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);


	NormalSpeed = 600.0f;
	SprintSpeedMultplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintSpeedMultplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	
	MaxHealth = 100;
	Health = MaxHealth;
	bIsSprint = false;

}


void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP();
	CachedHUD();
	UpdateHealthBar();
}

void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
		{



			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Move
				);
			}
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::StartJump
				);
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::EndJump
				);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Look
				);
			}
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::StartSprint
				);
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopSprint
				);
			}
		}
	}
}

float ASpartaCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvnet, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvnet, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UpdateOverheadHP();
	UpdateHealthBar();

	if (Health <= 0.0f)
	{
		OnDeath();
	}

	if (CachedHUDWidget)
	{
		UFunction* DamageFunc = CachedHUDWidget->FindFunction(FName("PlayDamageAnim"));
		if (DamageFunc)
		{
			CachedHUDWidget->ProcessEvent(DamageFunc, nullptr);

		}
	}

	return ActualDamage;
}

void ASpartaCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}

}

void ASpartaCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void ASpartaCharacter::EndJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void ASpartaCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);

}

void ASpartaCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}

	if (!bIsSprint)
	{
		bIsSprint = true;
		if (CachedHUDWidget)
		{
			UFunction* StartFunc = CachedHUDWidget->FindFunction(FName("StartSprint"));
			if (StartFunc)
			{
				CachedHUDWidget->ProcessEvent(StartFunc, nullptr);

			}
		}
	}

}

void ASpartaCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}

	if (bIsSprint)
	{
		bIsSprint = false;
		if (CachedHUDWidget)
		{
			UFunction* StopFunc = CachedHUDWidget->FindFunction(FName("StopSprint"));
			if (StopFunc)
			{
				CachedHUDWidget->ProcessEvent(StopFunc, nullptr);
			}
		}
	}


}

void ASpartaCharacter::OnDeath()
{
	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->OnGameOver();
	}
}

void ASpartaCharacter::UpdateOverheadHP()
{
	if (!OverheadWidget) return;
	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
	}

}

void ASpartaCharacter::UpdateHealthBar()
{
	if (!CachedHUDWidget) return;
	if (!HealthBar)
	{
		HealthBar = Cast<UProgressBar>(CachedHUDWidget->GetWidgetFromName(TEXT("HealthBar")));
	}
	HealthBar->SetPercent(Health / MaxHealth);
}

void ASpartaCharacter::CachedHUD()
{
	if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
	{

			CachedHUDWidget = PlayerController->GetHUDWidget();
			if (CachedHUDWidget)
			{
				HealthBar = nullptr;
				UpdateHealthBar();
			}
	}


}

void ASpartaCharacter::AddMaxHP(float Amount)
{
	MaxHealth += Amount;
	Health += Amount;
}

void ASpartaCharacter::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0, MaxHealth);
	UpdateOverheadHP();
	UpdateHealthBar();
}

float ASpartaCharacter::GetHealth() const
{
	return Health;
}

