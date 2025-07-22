// Fill out your copyright notice in the Description page of Project Settings.


#include "NbchwPawn.h"
#include "Components\CapsuleComponent.h"
#include "Camera\CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "SpartaPlayerController.h"
#include "GameFramework\SpringArmComponent.h"

// Sets default values
ANbchwPawn::ANbchwPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleCollision->SetCollisionProfileName("Pawn");
	SetRootComponent(CapsuleCollision);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(CapsuleCollision);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300;
	SpringArmComp->bUsePawnControlRotation = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	MoveSpeed = 400.0f;
}

// Called when the game starts or when spawned
void ANbchwPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ANbchwPawn::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();
	MovementInput = FVector(MoveInput.X, MoveInput.Y, 0.0f);
}

void ANbchwPawn::StopMove(const FInputActionValue& value)
{
	MovementInput = FVector::ZeroVector;
}

void ANbchwPawn::Look(const FInputActionValue& value)
{

	FVector2D LookInput = value.Get<FVector2D>();

	/*
	FRotator LookRotation = FRotator(0.0f, LookInput.X * 2.0f, 0.0f);
	if (SpringArmComp)
	{
		FRotator CurrentRotation = SpringArmComp->GetRelativeRotation();
		CurrentRotation.Pitch = FMath::Clamp(CurrentRotation.Pitch - LookInput.Y * 2.0f, -80.0f, 80.0f);
		SpringArmComp->SetRelativeRotation(CurrentRotation);
	}

	AddActorLocalRotation(LookRotation);
	*/
	AddActorLocalRotation(FRotator(0.f, LookInput.X * 15.0f * GetWorld()->GetDeltaSeconds(), 0.f));
	FRotator currentspringarmrotation = SpringArmComp->GetRelativeRotation();
	float newpitch = currentspringarmrotation.Pitch - LookInput.Y * 15.0f * GetWorld()->GetDeltaSeconds();

	newpitch = FMath::Clamp(newpitch, -80.f, 80.f);

	SpringArmComp->SetRelativeRotation(FRotator(newpitch, 0.f, 0.f));





}

// Called every frame
void ANbchwPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MovementInput.IsNearlyZero())
	{

		FVector Forward = CameraComp->GetForwardVector();
		FVector Right = CameraComp->GetRightVector();


		FVector Moving = Forward * MovementInput.X + Right * MovementInput.Y;
		Moving.Z = 0.f;

		AddActorWorldOffset(Moving.GetSafeNormal() * MoveSpeed * DeltaTime, true);
	}
}

// Called to bind functionality to input
void ANbchwPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
					&ANbchwPawn::Move
				);
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Completed,
					this,
					&ANbchwPawn::StopMove
				);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ANbchwPawn::Look
				);
			}
			
		}
	}

}