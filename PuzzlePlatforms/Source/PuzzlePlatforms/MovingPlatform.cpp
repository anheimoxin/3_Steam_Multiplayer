// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	SetMobility(EComponentMobility::Movable);

	if (HasAuthority()) {
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	GlobalStartLocation = GetActorLocation();
	GlobalEndLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::AddTrigger()
{
	ActivedTrigger++;
}

void AMovingPlatform::RemoveTrigger()
{
	if (ActivedTrigger > 0)
	{
		ActivedTrigger--;
	}

}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ActivedTrigger > 0)
	{
		if (HasAuthority())
		{
			FVector Location = GetActorLocation();
			float MovingLength = (GlobalEndLocation - GlobalStartLocation).Size();
			float MovingJourney = (Location - GlobalStartLocation).Size();

			if (MovingJourney >= MovingLength)
			{
				FVector swap = GlobalEndLocation;
				GlobalEndLocation = GlobalStartLocation;
				GlobalStartLocation = swap;
			}
			FVector Direction = (GlobalEndLocation - GlobalStartLocation).GetSafeNormal();
			Location += speed * DeltaSeconds * Direction;
			SetActorLocation(Location);
		}
	}
}


