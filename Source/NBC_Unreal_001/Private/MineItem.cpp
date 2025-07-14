// Fill out your copyright notice in the Description page of Project Settings.


#include "MineItem.h"
#include "Kismet/GameplayStatics.h"
#include "Particles\ParticleSystemComponent.h"
#include "Components/SphereComponent.h"

AMineItem::AMineItem()
{
	ExplosionDelay = 5.0f;
	ExplosionRadius = 300.0f;
	ExplosionDamage = 30;

	ExCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExCollision"));
	ExCollision->InitSphereRadius(ExplosionRadius);
	ExCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExCollision->SetupAttachment(Scene);
	bHasExploded = false;

	ItemType = "Mine";
}

void AMineItem::ActivateItem(AActor* Activator)
{
	if (bHasExploded) return;


	// 게임월드 > 타이머 매니저 
	// 타이머 핸들러

	Super::ActivateItem(Activator);
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AMineItem::Explode, ExplosionDelay, false);
	bHasExploded = true;

}

void AMineItem::Explode()
{
	UParticleSystemComponent* Particle = nullptr;
	if (ExplosionParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), GetActorRotation(), false);
	}
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}

	TArray<AActor*> OverlappingActors;
	ExCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				Actor,
				ExplosionDamage,
				nullptr,
				this,
				UDamageType::StaticClass()
				);
		}
	}

	DestroyItem();
	/*
	if (Particle)
	{
		FTimerHandle DestroyParticleTImerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTImerHandle,
			[Particle]()
			{
				Particle->DestroyComponent();
			},
			2.0f,
			false
		);
	}
	*/
}
