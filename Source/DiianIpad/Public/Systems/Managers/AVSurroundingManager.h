// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "Systems/Managers/AVManagerBase.h"
#include "AVSurroundingManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSBForceHiddenSignature, UClass*, Class, bool, bForceHidden);

USTRUCT()
struct FPOIInfo
{
	GENERATED_BODY()
public:
	FPOIInfo()
		:bHidden(false),
		TriggerPoint(-1.f),
		RegisteredActor(TSet<AActor*>()),
		Class(nullptr),
		bForceHidden(false)
	{}

	uint8 bHidden : 1;
	float TriggerPoint;
	TSet<AActor*> RegisteredActor;
	UClass* Class;
	uint8 bForceHidden : 1;

	void HideActor(bool bShouldHide = false)
	{
		if (bHidden == bShouldHide) return;

		for (AActor* POI : RegisteredActor)
		{
			POI->SetActorHiddenInGame(bShouldHide);
		}

		bHidden = bShouldHide;
	}
};

/**
 * 
 */
UCLASS()
class DIIANIPAD_API AAVSurroundingManager : public AAVManagerBase
{
	GENERATED_BODY()

public:
	AAVSurroundingManager(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category = "AV|Surrounding")
	void RegisterSurroundingComponent(AActor * InActor);

	UFUNCTION(BlueprintPure, Category= "AV|Surrounding")
	void GetActorSetFromClass(UClass* InClass, TSet<AActor*> & OutSet);

	UFUNCTION(BlueprintCallable, Category = "AV|Surrounding")
	void ForceHideActorsFromClass(UClass* InClass, bool bShouldHide = false);

	UFUNCTION(BlueprintCallable, Category = "AV|Surrounding")
	void ToggleForceHideActorsFromClass(UClass* InClass);

	void SetClassForceHidden(UClass * InClass, bool bForceHidden = false);

	UPROPERTY(BlueprintAssignable, Category="AV")
	FSBForceHiddenSignature OnForceHiddenChanges;
	
	FPOIInfo* GetPOIInfo(UClass* InClass) 
	{
		FPOIInfo * Info = nullptr;

		if (AllRegisteredActors.Contains(InClass))
		{
			Info = AllRegisteredActors[InClass];
		}
		else
		{
			for(const TPair<UClass*, FPOIInfo*> & Pair : AllRegisteredActors)
			{
				if(!Pair.Key->IsChildOf(InClass)) continue;

				Info = Pair.Value;
				break;
			}
		}

		return Info;
	}

protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End of AActor Interface

	void CheckPlayerCameraPitch(); 
	void SetHiddenAllRegisterActors(bool bShouldHide = false);
	
	void OnActorForceHidden(class AAVSurroundingBase * SB, bool bShouldHide);

private:
	UPROPERTY(EditAnywhere, Category="Surrounding", meta = (DisplayPriority = 1))
	float CheckPitchSubStep;

	TMap<UClass*, FPOIInfo*> AllRegisteredActors; 

	FTimerHandle TimerHandlerPitchSubStep;
};
