// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TouchComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIIANIPAD_API UTouchComponent : public UActorComponent
{
	GENERATED_BODY()
//
//public:	 
//	UTouchComponent(const class FObjectInitializer& ObjectInitializer);
//
//	// Returns the Touch Movement Component
//	UFUNCTION(BlueprintPure, Category = "AV|Touch")
//	static UTouchComponent* FindTouchComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UTouchComponent>() : nullptr); }
//
//	//~ UControllerComponent Interface  
//	virtual void OnRegister() override;
//	//~ End of UControllerComponent Interface 
//
//	UFUNCTION(BlueprintPure, Category = "AV|Touch")
//	FVector GetTouchDelta(uint8 FingerIndex);
//	UFUNCTION(BlueprintPure, Category = "AV|Touch")
//	FVector GetTouchDeltaNormalized(uint8 FingerIndex);
//
//	UFUNCTION(BlueprintPure, Category = "AV|Touch")
//	FVector GetTouchStartLocation(uint8 FingerIndex);
//
//	UFUNCTION(BlueprintPure, Category = "AV|Touch")
//	uint8 GetTouchFingerIndexBitMap() const { return TouchFingerIndex; }
//	
//public:
//	FTouchSignature OnTouchStarted;
//	FTouchSignature OnTouchEnded;
//	
//	FTouchSignature OnDoubleTap;
//
//protected: 
//	void TryInitializePlayerInput();
//	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);
//	
//	void TouchStart(ETouchIndex::Type FingerIndex, FVector Location);
//	void TouchEnd(ETouchIndex::Type FingerIndex, FVector Location);
//	void TouchMove(ETouchIndex::Type FingerIndex, FVector Location);
//
//	void DoubleTap(ETouchIndex::Type FingerIndex, FVector Location);
//	 
//	float AcceptableDelay;
//	
//private: 
//	float DoubleTapTimer;
//	uint8 TapCounter;
//
//	TMap<uint8, float> TouchStartTimes;
//	TMap<uint8, float> TouchEndTimes;
//	TMap<uint8, FVector> TouchStartLocations;
//	TMap<uint8, FVector> TouchCurrentLocations;
//
//	uint8 TouchFingerIndex;
//	  
};
