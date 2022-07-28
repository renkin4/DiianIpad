// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AVSurroundingBase.generated.h"

class AAVSurroundingManager;
class AAVSurroundingBase;

DECLARE_MULTICAST_DELEGATE_TwoParams(FForceHiddenSignature, AAVSurroundingBase* SB, bool bForceHidden);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPForceHiddenSignature, bool, bForceHidden);


UCLASS()
class DIIANIPAD_API AAVSurroundingBase : public AActor
{
	GENERATED_BODY()
	
public:
	AAVSurroundingBase(const class FObjectInitializer & ObjectInitializer);

	virtual float GetTriggerPoint() const;
	virtual void SetForceHidden(bool bShouldHide);

	UFUNCTION(BlueprintPure, Category = "AV")
	virtual FString GetPOIName_Implementation();

	UFUNCTION(BlueprintCallable, Category="Utils", meta = (DisplayName = "Set Force Hidden"))
	void K2_SetForceHidden(bool bShouldHide = true);

	UFUNCTION(BlueprintCallable, Category="Utils")
	void ToggleForceHidden();

	UFUNCTION(BlueprintImplementableEvent, Category = "Utils", meta= (DisplayName="Custom Set Actor Hidden"))
	void K2_CustomSetActorHidden(bool bShouldHide = false);
	
	virtual void CustomSetActorHidden(bool bShouldHide = false);

	FForceHiddenSignature OnForceHidden;

	UPROPERTY(BlueprintAssignable, Category="Utils")
	FBPForceHiddenSignature K2_ForceHidden;

	UFUNCTION(BlueprintPure, Category = "Utils")
	bool GetForceHidden() const { return bForceHidden; }

protected:
	//~ Begin AActor Interface override
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	//~ End of AActor Interface override

	void TryRegisterToSurroundingManager();
	void TryGetPawn();

	virtual void OnFinishedRegistering() {}

protected:
	// I Changed it to only register this point on startup
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Utils", meta = (DisplayPriority = 1))
	float TriggerPoints;

	static APawn * S_SurroundingCachedPawn; 
	uint8 bForceHidden : 1; 

	AAVSurroundingManager * SurroundingManger;

	UPROPERTY(EditAnywhere, Category = "Utils", meta = (DisplayPriority = 1))
	uint8 bCustomHidden : 1;

public:
};
