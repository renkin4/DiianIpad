// Powered By Yang
 
#include "Player/Components/PlayerUIComponent.h"

UPlayerUIComponent::UPlayerUIComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

bool UPlayerUIComponent::ToggleTag(const FGameplayTag& NewTag)
{
	bool bResult = RegisterNewTag(NewTag);

	if (!bResult)
	{
		bResult = UnregisterTag(NewTag);
		check(bResult);
	}

	return bResult;
}

bool UPlayerUIComponent::RegisterNewTag(const FGameplayTag& NewTag)
{
	if (PlayerUIGameplayTag.HasTag(NewTag))
	{
		return false; // Tag Existed
	}

	PlayerUIGameplayTag.AddTag(NewTag);
	OnGameplayTagChanges.Broadcast(PlayerUIGameplayTag);
	return true;
}

bool UPlayerUIComponent::UnregisterTag(const FGameplayTag& TagToRemove)
{
	if (!PlayerUIGameplayTag.HasTag(TagToRemove))
	{
		return false; // Tag Existed
	}

	PlayerUIGameplayTag.RemoveTag(TagToRemove);
	OnGameplayTagChanges.Broadcast(PlayerUIGameplayTag);
	return true;

}

void UPlayerUIComponent::RegisterContainer(const FGameplayTagContainer& NewContainer)
{
	PlayerUIGameplayTag.AppendTags(NewContainer);
}

void UPlayerUIComponent::UnregisterFromContainer(const FGameplayTagContainer& TagsToRemove)
{
	PlayerUIGameplayTag.RemoveTags(TagsToRemove);
}

