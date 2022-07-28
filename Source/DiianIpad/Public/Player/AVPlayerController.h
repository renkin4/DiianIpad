// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AVPlayerController.generated.h"

class UUserWidget;
class UPlayerUIComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTouchSignature, uint8, FingerIndex);

/**
 * 
 */
UCLASS()
class DIIANIPAD_API AAVPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAVPlayerController(const class FObjectInitializer & ObjectInitializer);
	
	UFUNCTION(BlueprintPure, Category = "AV|Touch")
	FVector GetTouchDelta(uint8 FingerIndex);
	UFUNCTION(BlueprintPure, Category = "AV|Touch")
	FVector GetTouchDeltaNormalized(uint8 FingerIndex);

	UFUNCTION(BlueprintPure, Category = "AV|Touch")
	FVector GetTouchStartLocation(uint8 FingerIndex);

	UFUNCTION(BlueprintPure, Category = "AV|Touch")
	uint8 GetTouchFingerIndexBitMap() const { return TouchFingerIndex; }

public:
	FTouchSignature OnTouchStarted;
	FTouchSignature OnTouchEnded;

	FTouchSignature OnDoubleTap;
protected:
	//~ Begin Override
	virtual void BeginPlay() override;  
	virtual void SetupInputComponent() override; 
	//~ End Override
	
	void TouchStart(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchEnd(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchMove(ETouchIndex::Type FingerIndex, FVector Location);

	void DoubleTap(ETouchIndex::Type FingerIndex, FVector Location);

	float AcceptableDelay;

private:
	float DoubleTapTimer;
	uint8 TapCounter;

	TMap<uint8, float> TouchStartTimes;
	TMap<uint8, float> TouchEndTimes;
	TMap<uint8, FVector> TouchStartLocations;
	TMap<uint8, FVector> TouchCurrentLocations;

	uint8 TouchFingerIndex;

protected:
	UPROPERTY(EditAnywhere, Category = "AV")
	uint8 bShouldCreateMenuList : 1;

private:
	UPROPERTY()
	UPlayerUIComponent* PlayerUIComp;
	
	UPROPERTY()
	TSubclassOf< UUserWidget> MenuListClass;

	UPROPERTY(Transient)
	UUserWidget * MenuList;
	
	UPROPERTY(Transient)
	class UTouchComponent* TouchComponent;

};
