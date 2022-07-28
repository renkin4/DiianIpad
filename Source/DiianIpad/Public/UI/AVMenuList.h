// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "UI/AVTaggedWidget.h"
#include "AVMenuList.generated.h"

/**
 *
 */
UCLASS()
class DIIANIPAD_API UAVMenuList : public UAVTaggedWidget
{
	GENERATED_BODY()

public:
	UAVMenuList(const class FObjectInitializer& ObjectInitializer);

protected:
	//~ Begin
	virtual void NativeConstruct() override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;  
	//~ End 

	//~ Begin
	void OnWatchedTagsChanges(const FGameplayTagContainer& GameplayTagContainer) override;
	//~ End

	void HideMenuOnStart();
	void TryToggleMenu(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);
	void OnFinishBlending();
	void ReturnBackToPosition();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "AV Menu")
	class UOverlay* OverlayWrapper;



private:
	float TouchStartTime;
	float TouchEndTime;

	FVector2D StartTouchLocation;
	FVector2D CurrentTouchLocation;

	UPROPERTY(EditAnywhere, Category = "Touch Utils")
	float AcceptableDelay;

	uint8 bTouchStart : 1;
	uint8 bHide : 1;

	UPROPERTY(EditAnywhere, Category = "AV Menu")
	float BlendTime;

	UPROPERTY(EditAnywhere, Category = "AV Menu")
	float BlendExponent;

	uint8 bStartBlend : 1;
	float BlendAlpha;
	float BlendWeight;
	uint8 bInitBlend : 1;

	FVector2D InitialLoc;
	FVector2D TargetLoc;

	const FGeometry* CacheGeo;
};
