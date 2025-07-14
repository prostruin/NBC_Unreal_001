// Fill out your copyright notice in the Description page of Project Settings.


#include "RotationActor.h"
#include "Kismet\GameplayStatics.h"
#include "Components\BoxComponent.h"

// Sets default values
ARotationActor::ARotationActor()
{

	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionProfileName("OverlapAllDynamic");
	BoxCollision->SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ARotationActor::OnRotationOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ARotationActor::OnRotationEndOverlap);

	RotationSpeed = 90;


	
}

// Called when the game starts or when spawned
void ARotationActor::BeginPlay()
{
	Super::BeginPlay();

}

void ARotationActor::OnRotationOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ARotationActor::OnRotationEndOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called every frame
void ARotationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

		AddActorLocalRotation(FRotator(0, RotationSpeed * DeltaTime, 0));

}

