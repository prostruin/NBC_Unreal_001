// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingActor.h"
#include "Kismet\GameplayStatics.h"
#include "Components\BoxComponent.h"

// Sets default values
AMovingActor::AMovingActor()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionProfileName("OverlapAllDynamic");
	BoxCollision->SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AMovingActor::OnRotationOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AMovingActor::OnRotationEndOverlap);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Scene);

	HideDelay = 3.0f;
	MoveSpeed = 5.0f;
	MaxRange = 250.0f;
	MoveDirection = 1;
	IshiddenBox = false;
	IsActorHidden = false;
	isdistance = false;
	isoverlap = false;
}

void AMovingActor::Hide()
{
	if (IshiddenBox)
	{
		if (IsActorHidden)
		{
			SetActorHiddenInGame(false);
			IsActorHidden = false;
		}
		else
		{

			SetActorHiddenInGame(true);
			IsActorHidden = true;
		}
	}
}

// Called when the game starts or when spawned
void AMovingActor::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
	if (IshiddenBox)
	{
		GetWorld()->GetTimerManager().SetTimer(HideActor, this, &AMovingActor::Hide, HideDelay, true);
	}
}

void AMovingActor::OnRotationOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	isoverlap = true;

}

void AMovingActor::OnRotationEndOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isoverlap = false;
}

// Called every frame
void AMovingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isoverlap)
	{
		if (!IshiddenBox)
		{


			FVector MoveAmount = Arrow->GetForwardVector() * MoveDirection * MoveSpeed * DeltaTime;
			FVector CurrentLocation = GetActorLocation();
			FVector NewLocation = CurrentLocation + MoveAmount;
			float Distance = FVector::Dist(StartLocation, NewLocation);

			if (Distance >= MaxRange)
			{
				MoveDirection *= -1.0f;
				isdistance = true;
			}

			if (isdistance)
			{
				if (Distance <= 1)
				{
					MoveDirection *= -1.0f;
					isdistance = false;
				}
			}

			SetActorLocation(NewLocation);
			UE_LOG(LogTemp, Warning, TEXT("위치값: %.8f"), Distance);
		}
	}
}

