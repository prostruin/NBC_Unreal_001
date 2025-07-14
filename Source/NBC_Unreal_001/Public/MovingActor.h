// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components\ArrowComponent.h"
#include "MovingActor.generated.h"

class UBoxComponent;

UCLASS()
class NBC_UNREAL_001_API AMovingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move|Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move|Component")
	UBoxComponent* BoxCollision;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arrow")
	UArrowComponent* Arrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|Speed")
	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|Speed")
	float MaxRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|Select")
	bool IshiddenBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|Select")
	bool isoverlap;

	FTimerHandle HideActor;
	void Hide();
	bool IsActorHidden;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|Hide")
	float HideDelay;

	FVector ForwardVector;
	FVector StartLocation;
	UPROPERTY(VisibleAnywhere, Category = "Move")
	int32 MoveDirection;

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
	bool isdistance;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
