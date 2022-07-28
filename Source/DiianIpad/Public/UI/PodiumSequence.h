// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "UI/AVUserWidget.h"
#include "PodiumSequence.generated.h"

class UPodiumHitbox;
class UImage;
class UCanvasPanel;
class AAVPodiumManager;

/**
 * 
 */
UCLASS()
class DIIANIPAD_API UPodiumSequence : public UAVUserWidget
{
	GENERATED_BODY()
	
public:
	UPodiumSequence(const class FObjectInitializer & ObjectInitializer);

protected:
	void Init();

	//~ Begin Override
	virtual void NativePreConstruct() override; 
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	//~ Begin Override

	void TryGetPodiumManager();
	void ProjectHitBoxes();

	UFUNCTION()
	void OnFinishLoadingTextures();

	UFUNCTION()
	void OnPodiumIndexChanges(int32 NewIndex, int32 OldIndex);

	UFUNCTION()
	void OnPodiumChangeStates(EPodiumState NewState, EPodiumState OldState);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "AV")
	UImage* SequenceImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "AV")
	UCanvasPanel* SequenceCanvas;

private:
	UPROPERTY(EditAnywhere, Category= "AV")
	TSubclassOf<UPodiumHitbox> HitBoxClass;

	UPROPERTY(Transient)
	AAVPodiumManager * PodiumManager;

	TArray<AActor*> AllDius;
	TArray<UPodiumHitbox *> AllPodiumHitboxes;

};
