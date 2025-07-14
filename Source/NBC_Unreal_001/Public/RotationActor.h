// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components\ArrowComponent.h"
#include "RotationActor.generated.h"

class UBoxComponent;

UCLASS()
class NBC_UNREAL_001_API ARotationActor : public AActor
{
	GENERATED_BODY()
	
public:	

	ARotationActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rotation|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rotation|Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rotation|Component")
	UBoxComponent* BoxCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation|Speed")
	int32 RotationSpeed;


	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnRotationOverlap(
		UPrimitiveComponent* OverlapComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnRotationEndOverlap(
		UPrimitiveComponent* OverlapComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);



public:	

	virtual void Tick(float DeltaTime) override;

};
