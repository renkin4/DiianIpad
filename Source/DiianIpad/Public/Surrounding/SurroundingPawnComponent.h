// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TourComponent.h"
#include "SurroundingPawnComponent.generated.h"

/**
 * 
 */
UCLASS()
class DIIANIPAD_API USurroundingPawnComponent : public UTourComponent
{
	GENERATED_BODY()
	
public:
	USurroundingPawnComponent(const class FObjectInitializer& ObjectInitializer);
	 
protected:
	//~ Begin UActorComponent Interface
	virtual void OnRegister() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~ End of UActorComponent Interface

	virtual void RotatePawn(float DeltaTime) override;

	void PanPawn(float DeltaTime);
	bool CanPan() const;

	void CheckPawnPosition(float DeltaTime);
	void RotatePawnToNorth(float DeltaTime);

	virtual void OnDoubleTap(uint8 FingerIndex) override;

	void StartPitching();

private:
	void RotatePawnNextTick();

	FVector PanAABB(const FVector& NewLoc);

private:
	FVector OwnerInitialLocation;

	uint8 bIsPanning : 1;

	UPROPERTY(EditAnywhere, Category=Aiyaya)
	float PanningSpeed;

	UPROPERTY(EditAnywhere, Category=Aiyaya)
	FVector PanBoxOrigin;
	UPROPERTY(EditAnywhere, Category=Aiyaya)
	FVector PanBoxExtend;

	FVector TargetPanLocation;
	 
	float AmountToPitch;
	float ConstAmountToPitch;
	uint8 bStartPitchingUp : 1;
};
