// Powered By Yang

#pragma once
 
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "AVTaggedWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWatchTagSignature, bool, bHide);

/**
 *
 */
UCLASS()
class DIIANIPAD_API UAVTaggedWidget : public UUserWidget
{
GENERATED_BODY()

public:
	UAVTaggedWidget(const class FObjectInitializer& ObjectInitializer);
 
	//~ UUserWidget Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//~ End of UUserWidget Interface 

	UPROPERTY(BlueprintAssignable, Category = "HUD", meta = (DisplayPriority = 1)) 
	FWatchTagSignature OnWatchTagChanges;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (DisplayPriority = 1))
	FGameplayTagContainer HiddenByTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (DisplayPriority = 1))
	ESlateVisibility ShownVisibility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (DisplayPriority = 1))
	ESlateVisibility HiddenVisibility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (DisplayPriority = 1))
	uint8 bManualHandleVisibility : 1;

	UFUNCTION()
	virtual void OnWatchedTagsChanges(const FGameplayTagContainer& GameplayTagContainer);

private:
	virtual void BindFunctionsFromPlayer();
	virtual void UnbindFunctionsFromPlayer();
	
	
};