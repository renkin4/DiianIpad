// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HFloorPlan.h"
#include "HFloorPlanImage.generated.h"

class UImage;
 
/**
 * 
 */
UCLASS()
class DIIANIPAD_API UHFloorPlanImage : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UHFloorPlanImage(const class FObjectInitializer & ObjectInitializer);

	// dependency ? Who Care about compilation or proper codding practices
	void Init(int32 CellIndex, FFloorPlanImageInfo* Info);
private: 
	int32 Index;
	FFloorPlanImageInfo * CellInfo;

protected:
	//~ Begin
	virtual void NativePreConstruct() override;
	//~ End 

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "AV")
	UImage* FloorPlanImage;
};
