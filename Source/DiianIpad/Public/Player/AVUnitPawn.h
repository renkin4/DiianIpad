// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/AVPawn.h"
#include "AVUnitPawn.generated.h"

class AAVUnitManager;

/**
 * 
 */
UCLASS()
class DIIANIPAD_API AAVUnitPawn : public AAVPawn
{
	GENERATED_BODY()
	
public:
	AAVUnitPawn(const class FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual TSubclassOf<UAVCameraMode> DetermineCameraMode() const override;

	void TryGetUnitManager();

private:
	UPROPERTY(Transient)
	AAVUnitManager * UnitManager;

	UPROPERTY(Transient)
	class UTourComponent* TourComponent;

};
