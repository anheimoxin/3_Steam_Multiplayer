// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMovingPlatform();

	virtual void Tick(float DeltaSeconds);
	virtual void BeginPlay();

	UPROPERTY(EditAnywhere)
	float speed = 100.0f;

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector TargetLocation;

	void AddTrigger();
	void RemoveTrigger();

protected:

	UPROPERTY(EditAnywhere)
		int ActivedTrigger = 1;

	FVector GlobalStartLocation;
	FVector GlobalEndLocation;
};
