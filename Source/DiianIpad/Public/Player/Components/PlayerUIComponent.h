// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ControllerComponents.h"
#include "PlayerUIComponent.generated.h"
 
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FGameplayTagSignature, UPlayerUIComponent, OnGameplayTagChanges, const FGameplayTagContainer&, Tag);

/**
 *
 */
UCLASS()
class DIIANIPAD_API UPlayerUIComponent : public UControllerComponents
{
	GENERATED_BODY()

public:
	UPlayerUIComponent(const class FObjectInitializer& ObjectInitializer);
	
	// Returns the Touch Movement Component
	UFUNCTION(BlueprintPure, Category = "AV|UI")
	static UPlayerUIComponent* FindPlayerUIComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UPlayerUIComponent>() : nullptr); }

	UPROPERTY(BlueprintAssignable, Category="AV|UI")
	FGameplayTagSignature OnGameplayTagChanges;

	UFUNCTION(BlueprintCallable, Category="AV|UI")
	bool ToggleTag(const FGameplayTag & NewTag);
	
	UFUNCTION(BlueprintCallable, Category="AV|UI")
	bool RegisterNewTag(const FGameplayTag & NewTag);
	UFUNCTION(BlueprintCallable, Category="AV|UI")
	bool UnregisterTag(const FGameplayTag & TagToRemove);

	UFUNCTION(BlueprintCallable, Category="AV|UI")
	void RegisterContainer(const FGameplayTagContainer & NewContainer);
	UFUNCTION(BlueprintCallable, Category="AV|UI")
	void UnregisterFromContainer(const FGameplayTagContainer & TagsToRemove);

	FGameplayTagContainer GetPlayerUIGameplayTag() const { return PlayerUIGameplayTag; }

private:
	// Hidden tags
	UPROPERTY(EditAnywhere, Category = "AV|Tag", meta = (Categories = "UI.Hidden", DisplayPriority = 1))
	FGameplayTagContainer PlayerUIGameplayTag;
	
};
