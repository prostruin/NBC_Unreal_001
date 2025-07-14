// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"

/**
 * 
 */
UCLASS()
class NBC_UNREAL_001_API AMineItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AMineItem();

	bool bHasExploded;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* ExCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	UParticleSystem* ExplosionParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
	USoundBase* ExplosionSound;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ExplosionDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ExplosionDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ExplosionRadius;

	FTimerHandle ExplosionTimerHandle;

	virtual void ActivateItem(AActor* Activator) override;
	
	void Explode();
};
