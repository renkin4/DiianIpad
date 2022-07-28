// Powered By Yang

#include "UI/AVTaggedWidget.h"
#include "Player/Components/PlayerUIComponent.h"
 
UAVTaggedWidget::UAVTaggedWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	ShownVisibility = ESlateVisibility::Visible;
	HiddenVisibility = ESlateVisibility::Collapsed;
	bManualHandleVisibility = false;
}

void UAVTaggedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!IsDesignTime())
	{
		BindFunctionsFromPlayer();
	}
}

void UAVTaggedWidget::NativeDestruct()
{
	if (!IsDesignTime())
	{
		UnbindFunctionsFromPlayer();
	}

	Super::NativeDestruct();
}

void UAVTaggedWidget::BindFunctionsFromPlayer()
{
	APlayerController* PC = GetOwningPlayer();
	check(PC);

	UPlayerUIComponent* PlayerUIComponent = UPlayerUIComponent::FindPlayerUIComponent(PC);

	if (!PlayerUIComponent)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::BindFunctionsFromPlayer);
		return;
	}

	PlayerUIComponent->OnGameplayTagChanges.AddDynamic(this, &ThisClass::OnWatchedTagsChanges);
}

void UAVTaggedWidget::UnbindFunctionsFromPlayer()
{
	UPlayerUIComponent* PlayerUIComponent = UPlayerUIComponent::FindPlayerUIComponent(GetOwningPlayer());

	if (!PlayerUIComponent) return;

	PlayerUIComponent->OnGameplayTagChanges.RemoveDynamic(this, &ThisClass::OnWatchedTagsChanges);
}

void UAVTaggedWidget::OnWatchedTagsChanges(const FGameplayTagContainer& GameplayTagContainer)
{
	const UPlayerUIComponent* PlayerUIComponent = UPlayerUIComponent::FindPlayerUIComponent(GetOwningPlayer());
	const bool bHasHiddenTags = PlayerUIComponent->GetPlayerUIGameplayTag().HasAny(HiddenByTags);

	OnWatchTagChanges.Broadcast(bHasHiddenTags);
	if (bManualHandleVisibility) return;

	// Actually apply the visibility
	const ESlateVisibility DesiredVisibility = !bHasHiddenTags ? ShownVisibility : HiddenVisibility;
	if (GetVisibility() != DesiredVisibility)
	{
		SetVisibility(DesiredVisibility);
	}
}
