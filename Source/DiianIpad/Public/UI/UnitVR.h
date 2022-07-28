// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/AVType.h"
#include "UnitVR.generated.h"

class UUnitDollhouseHitbox;

/**
 * 
 */
UCLASS()
class DIIANIPAD_API UUnitVR : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
protected:
	//~ Begin 
	virtual void NativeConstruct() override; 
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~ End 

	void TryGetUnitManager();

	void InitHitbox();

	UFUNCTION()
	void OnGFChanges(EUnitGameplayFeatures NewGF, EUnitGameplayFeatures OldGF);

	UFUNCTION()
	void OnStateChanges(EUnitManagerState NewState);

	UFUNCTION()
	void OnTourIndexChanges(int32 Index);

protected:
	UPROPERTY(EditAnywhere, Category = "AV")
	TSubclassOf<UUnitDollhouseHitbox> DollHouseHitboxClass;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "AV")
	class UCanvasPanel* UnitVRCanvas;

private:
	UPROPERTY(Transient)
	TArray<UUnitDollhouseHitbox * > AllHitboxes;

	UPROPERTY(Transient)
	class AAVUnitManager * UnitManager;

	TMap<int32, FVector> HitboxMap;

	int32 CurrentIndex;
};
