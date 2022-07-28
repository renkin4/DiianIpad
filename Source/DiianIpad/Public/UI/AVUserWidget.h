// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AVUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DIIANIPAD_API UAVUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UAVUserWidget(const class FObjectInitializer & ObjectInitializer);
	
protected:
	//~ Begin UUserWidget Interface
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override; 
	virtual FReply NativeOnTouchGesture(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	//~ End of UUserWidget Interface

	UFUNCTION(BlueprintPure, Category="Touch Utils")
	FVector2D GetDeltaTouch();

	UFUNCTION(BlueprintPure, Category="Touch Utils")
	FVector2D GetDeltaTouchNormalized();

	UFUNCTION(BlueprintImplementableEvent, Category="Touch Utils")
	void OnTouchClicked();
	
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere ,Category="Touch Utils")
	float TouchStartTime;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere ,Category="Touch Utils")
	float TouchEndTime;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere ,Category="Touch Utils")
	FVector2D StartTouchLocation;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere ,Category="Touch Utils")
	FVector2D CurrentTouchLocation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere ,Category="Touch Utils")
	float AcceptableDelay;
};
