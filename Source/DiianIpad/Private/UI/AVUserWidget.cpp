// Powered By Yang

#include "UI/AVUserWidget.h"

UAVUserWidget::UAVUserWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	TouchStartTime = 0.f;
	TouchEndTime = 0.f;

	AcceptableDelay = 0.15f;
	bIsFocusable = true;

	StartTouchLocation = FVector2D::ZeroVector;
	CurrentTouchLocation = FVector2D::ZeroVector;
}

FReply UAVUserWidget::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	const FReply& SuperReply = Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	if (SuperReply.IsEventHandled())
	{
		return SuperReply;
	}

	//GEngine->AddOnScreenDebugMessage(-1 , 2.f, FColor::Red, FString::Printf(TEXT("Finger Index : %d"), InGestureEvent.GetTouchpadIndex()));

	TouchStartTime = GetWorld()->GetTimeSeconds();
	StartTouchLocation = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());
	CurrentTouchLocation = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());
	return FReply::Handled();
}

FReply UAVUserWidget::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	const FReply& SuperReply = Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	if (SuperReply.IsEventHandled())
	{
		return SuperReply;
	}

	CurrentTouchLocation = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());
	return FReply::Handled();
}

FReply UAVUserWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	const FReply& SuperReply = Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	if (SuperReply.IsEventHandled())
	{
		return SuperReply;
	}

	TouchEndTime = GetWorld()->GetTimeSeconds();
	if (TouchStartTime + AcceptableDelay > TouchEndTime)
	{
		OnTouchClicked();
	}

	return FReply::Handled();
}

FVector2D UAVUserWidget::GetDeltaTouch()
{
	if (TouchStartTime <= TouchEndTime + AcceptableDelay) return FVector2D::ZeroVector;

	FVector2D Delta = CurrentTouchLocation - StartTouchLocation;

	return Delta;
}

FVector2D UAVUserWidget::GetDeltaTouchNormalized()
{
	return GetDeltaTouch().GetSafeNormal();
}

FReply UAVUserWidget::NativeOnTouchGesture(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Finger Gesuture : %d"), (uint8)InGestureEvent.GetGestureType()));

	return FReply::Handled();
}
