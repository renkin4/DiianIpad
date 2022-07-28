// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/AVUserWidget.h"
#include "UnitDollhouse.generated.h"

class AAVUnitManager;
class UImage;
class UUnitDollhouseHitbox;

/**
 * 
 */
UCLASS()
class DIIANIPAD_API UUnitDollhouse : public UAVUserWidget
{
	GENERATED_BODY()
	
public:
	UUnitDollhouse(const class FObjectInitializer & ObjectInitializer);

protected:
	//~ Begin Override 
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~ End Override 

	void TryUnitManager();

	void OnDollHouseIndexChanges(int32 NewIndex);
	void OnFinishLoadingDollhouse();

	UFUNCTION()
	void OnUnitStateChanges(EUnitManagerState NewState);

	UFUNCTION()
	void OnGFChanges(EUnitGameplayFeatures NewGF, EUnitGameplayFeatures OldGF);

	void InitHitboxes();
	 
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "AV")
	UImage* SequenceImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "AV")
	class UCanvasPanel* DollhouseCanvas;

	UPROPERTY(EditAnywhere, Category= "AV")
	TSubclassOf<UUnitDollhouseHitbox> TourHitboxClass;

private:
	UPROPERTY(Transient)
	AAVUnitManager* UnitManager;

	UPROPERTY(Transient)
	TArray<UUnitDollhouseHitbox*> AllHitboxes;
	
	TMap<int32, FVector> AllTourPositions;
};
