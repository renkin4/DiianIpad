// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AVUnitPawn.h"
#include "Systems/Managers/AVUnitManager.h"
#include "Camera/AVCameraMode.h"
#include "Components/TourComponent.h"

AAVUnitPawn::AAVUnitPawn(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	TourComponent = CreateDefaultSubobject<UTourComponent>(TEXT("Tour Component"));
	
	PrimaryActorTick.bCanEverTick = true;
}

void AAVUnitPawn::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::TryGetUnitManager);
}

TSubclassOf<UAVCameraMode> AAVUnitPawn::DetermineCameraMode() const
{
	TSubclassOf<UAVCameraMode> CM = UnitManager->GetCameraMode();
	if(!CM)
	{
		return DefaultCameraMode;
	}

	return CM;
}

void AAVUnitPawn::TryGetUnitManager()
{
	UnitManager = AAVUnitManager::Get<AAVUnitManager>();

	if(!UnitManager)
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::TryGetUnitManager);
		return;
	}
}
