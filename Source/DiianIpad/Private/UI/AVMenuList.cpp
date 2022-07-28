// Powered By Yang


#include "UI/AVMenuList.h"
#include "Player/Components/PlayerUIComponent.h"
#include "GameplayTagContainer.h"
#include "Components/Overlay.h"
#include "Kismet/KismetSystemLibrary.h"

#define HIDE_UI_TAG FGameplayTag::RequestGameplayTag("UI.Hidden.Menu")

UAVMenuList::UAVMenuList(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	AcceptableDelay = 0.125f;
	bHide = false;

	TouchStartTime = 0.f;
	TouchEndTime = 9999999999999.f;

	BlendTime = 1.f;
	BlendExponent = 4.f;

	BlendAlpha = 0.f;
	BlendWeight = 0.f;

	bStartBlend = false;

	HiddenByTags.AddTag(HIDE_UI_TAG);
	bIsFocusable = true;
}

FReply UAVMenuList::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	//CacheGeo = InGeometry;
	if (bStartBlend)
	{
		return FReply::Handled();
	}

	TouchStartTime = GetWorld()->GetTimeSeconds();

	StartTouchLocation = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());
	CurrentTouchLocation = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());

	bTouchStart = true;

	return FReply::Handled();
}

FReply UAVMenuList::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	if (bStartBlend)
	{
		return FReply::Handled();
	}

	if (!bTouchStart || bHide)
	{
		return FReply::Unhandled();
	}

	//CacheGeo = InGeometry;

	const FVector2D PreviousLoc = CurrentTouchLocation;
	CurrentTouchLocation = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());

	const FVector2D& LocalSize = GetPaintSpaceGeometry().GetLocalSize();

	const FVector2D Delta = CurrentTouchLocation - PreviousLoc;
	const FVector2D& TranslatedLoc = OverlayWrapper->RenderTransform.Translation;

	if (ensure(OverlayWrapper))
	{
		FVector2D NewLoc(FMath::Clamp(TranslatedLoc.X + Delta.X, (LocalSize.X * 0.975f) * -1, 0.f), 0.f);
		OverlayWrapper->SetRenderTranslation(NewLoc);
	}

	return FReply::Handled();
}

FReply UAVMenuList::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	//CacheGeo = InGeometry;
	if (bStartBlend)
	{
		return FReply::Handled();
	}

	TouchEndTime = GetWorld()->GetTimeSeconds();

	if (!bTouchStart)
	{
		bTouchStart = false;
		return FReply::Unhandled();
	}

	if (TouchStartTime + AcceptableDelay > TouchEndTime)
	{
		TryToggleMenu(InGeometry, InGestureEvent);
		bTouchStart = false;
		return FReply::Handled();
	}

	UPlayerUIComponent* PlayerUIComponent = UPlayerUIComponent::FindPlayerUIComponent(GetOwningPlayer());

	const FVector2D& LocalSize = GetPaintSpaceGeometry().GetLocalSize();
	const FVector2D& TranslatedLoc = FMath::Abs(OverlayWrapper->RenderTransform.Translation);

	const float MovedPercentage = (LocalSize.X - TranslatedLoc.X) / LocalSize.X;

	if (!bHide && MovedPercentage < 0.8f)
	{
		PlayerUIComponent->RegisterNewTag(HIDE_UI_TAG);
		bTouchStart = false;
		return FReply::Handled();
	}

	if (bHide && MovedPercentage > 0.2f)
	{
		PlayerUIComponent->UnregisterTag(HIDE_UI_TAG);
		bTouchStart = false;
		return FReply::Handled();
	}

	ReturnBackToPosition();
	bTouchStart = false;
	return FReply::Handled();
}

void UAVMenuList::OnWatchedTagsChanges(const FGameplayTagContainer& GameplayTagContainer)
{
	const UPlayerUIComponent* PlayerUIComponent = UPlayerUIComponent::FindPlayerUIComponent(GetOwningPlayer());
	const bool bHasHiddenTags = PlayerUIComponent->GetPlayerUIGameplayTag().HasAny(HiddenByTags);

	bHide = bHasHiddenTags;

	ReturnBackToPosition();

	OnWatchTagChanges.Broadcast(bHide);
}

void UAVMenuList::HideMenuOnStart()
{
	UPlayerUIComponent* PlayerUIComponent = UPlayerUIComponent::FindPlayerUIComponent(GetOwningPlayer());
	if (!PlayerUIComponent)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HideMenuOnStart);
		return;
	}

	PlayerUIComponent->RegisterNewTag(HIDE_UI_TAG);
	bTouchStart = false;

}

void UAVMenuList::TryToggleMenu(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	const FVector2D& TranslatedLoc = OverlayWrapper->RenderTransform.Translation;
	const FVector2D& LocalSize = InGeometry.GetLocalSize();

	const FVector2D& TouchLocation = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());

	const float CurrentPercentageOfDraw = FMath::Abs(LocalSize.X + TranslatedLoc.X) / LocalSize.X;
	UPlayerUIComponent* PlayerUIComponent = UPlayerUIComponent::FindPlayerUIComponent(GetOwningPlayer());

	const float TouchPercentage = (TouchLocation.X - TranslatedLoc.X) / (LocalSize.X);

	if (TouchPercentage >= 0.975f)
	{
		PlayerUIComponent->ToggleTag(HIDE_UI_TAG);
		return;
	}
}

void UAVMenuList::OnFinishBlending()
{
	bStartBlend = false;
}

void UAVMenuList::ReturnBackToPosition()
{
	const FVector2D& LocalSize = GetPaintSpaceGeometry().GetLocalSize();
	const FVector2D& TranslatedLoc = OverlayWrapper->RenderTransform.Translation;

	InitialLoc = FVector2D(TranslatedLoc.X, 0.f);

	if (bHide)
	{
		InitialLoc = FVector2D(0.f, 0.f);
		TargetLoc = FVector2D((LocalSize.X * .975f) * -1, 0.f);
		BlendAlpha = FMath::Abs(TranslatedLoc.X) / FMath::Abs(TargetLoc.X);
		const float a = FMath::Pow(((BlendAlpha * -1.f) + 1), 1 / BlendExponent) * -1;
		BlendAlpha = a + 1;
		bInitBlend = true;
		//bInitBlend = true;
	}
	else
	{
		// todo blend alpha on expo as well. On the other note na... Just not have pulling on hide
		TargetLoc = FVector2D(0.f, 0.f);
		BlendAlpha = 0.f;
	}

	bStartBlend = true;
} 

void UAVMenuList::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
	 
	UPlayerUIComponent* PlayerUIComponent = UPlayerUIComponent::FindPlayerUIComponent(GetOwningPlayer());

	const FVector2D& LocalSize = GetPaintSpaceGeometry().GetLocalSize();
	const FVector2D& TranslatedLoc = FMath::Abs(OverlayWrapper->RenderTransform.Translation);

	const float MovedPercentage = (LocalSize.X - TranslatedLoc.X) / LocalSize.X;

	if (!bHide && MovedPercentage < 0.8f)
	{
		PlayerUIComponent->RegisterNewTag(HIDE_UI_TAG);
		bTouchStart = false; 
		return;
	}

	if (bHide && MovedPercentage > 0.2f)
	{
		PlayerUIComponent->UnregisterTag(HIDE_UI_TAG);
		bTouchStart = false;
		return;
	}

	ReturnBackToPosition();
	bTouchStart = false;
}

void UAVMenuList::NativeConstruct()
{
	Super::NativeConstruct();

	FTimerHandle TimerHandler;

	//if(GetWorld()->IsPlayInEditor())
	//{
	//	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ThisClass::HideMenuOnStart, 0.5f, false);
	//	return;
	//}

	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ThisClass::HideMenuOnStart, 1.f, false);
}

void UAVMenuList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//CacheGeo = &MyGeometry;

	if (!bTouchStart && ensure(OverlayWrapper) && bStartBlend)
	{
		BlendAlpha += bInitBlend ? 0.f : (InDeltaTime / BlendTime);
		bInitBlend = false;
		BlendAlpha = FMath::Min(BlendAlpha, 1.0f);
		const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
		//BlendWeight = FMath::Clamp(BlendAlpha, 0.f, 1.0f);

		FVector2D NewLoc = FMath::Lerp(InitialLoc, TargetLoc, BlendWeight);

		OverlayWrapper->SetRenderTranslation(NewLoc);

		if (BlendAlpha >= 1.f)
		{
			OnFinishBlending();
		}
	}

	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Translation : %s"), *OverlayWrapper->RenderTransform.Translation.ToString()));
}
