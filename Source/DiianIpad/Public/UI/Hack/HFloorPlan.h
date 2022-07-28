// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/AVUserWidget.h"
#include "Data/AVType.h"
#include "Systems/Managers/AVUnitManager.h"
#include "HFloorPlan.generated.h"

class UHFloorPlanImage;
class UCanvasPanelSlot;
class AAVUnitManager;
class UCanvasPanel;
class UImage;
class UTextBlock;

struct FFloorPlanImageInfo
{
public:
	FFloorPlanImageInfo() = delete;
	FFloorPlanImageInfo(FUnitData * UnitData)
		:UnitData(UnitData),
		FloorPlanTex(nullptr),
		FloorPlanImage(nullptr),
		Slot(nullptr)
	{}

	FUnitData * UnitData;
	UTexture2D* FloorPlanTex; 

	FVector2D Size;
	float AspectRatio;
	UHFloorPlanImage * FloorPlanImage;
	UCanvasPanelSlot * Slot;

public:
	void LoadUnit();
	void RefreshSize();
};

/**
 * 
 */
UCLASS()
class DIIANIPAD_API UHFloorPlan : public UAVUserWidget
{
	GENERATED_BODY()
	
public:
	UHFloorPlan(const class FObjectInitializer & ObjectInitializer);
	 
	UPROPERTY(BlueprintAssignable, Category="AV")
	FSelectUnitDataSignature K2_OnSelectedUnitChanges;

protected:
	//~ Begin UUserWidget override
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override; 
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	//~ End UUserWidget override
	
	void RefreshTextureSizes();
	void InitCells();

	void ArrangeChild();
	void SetupManager();
	void TryToCenterSelected(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "AV")
	void OnSelectedUnitChanges(const FUnitData & UnitData);





protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "AV")
	UCanvasPanel * FloorPlanCanvas;
	 
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "AV")
	UTextBlock * TypeText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "AV")
	UTextBlock * TypeTextDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AV", meta=(DisplayPriority = 1))
	FVector2D CellPaddings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AV", meta=(DisplayPriority = 1))
	float MaxYScale;

	UPROPERTY(EditAnywhere, Category = "AV")
	float CenterInterpSpeed;
	
	UPROPERTY(EditAnywhere, Category = "AV")
	uint8 bCustomUnitData : 1;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "AV", meta=(DisplayPriority = 1))
	AAVUnitManager * UnitManager;

private:
	UPROPERTY(EditAnywhere, Category = "AV", meta=(DisplayPriority = 1))
	TSubclassOf<UHFloorPlanImage> FloorPlanImageClass;

	UPROPERTY(EditAnywhere, Category = "AV", meta = (DisplayPriority = 1))
	UDataTable * DT_UnitData;
	
private:
	float ScrollOffset;
	float AccumulatedSize;

	TMap<int32, FFloorPlanImageInfo*> AllFloorPlansInfo;
	 
	int32 CurrentIndex;
	int32 Direction;

};
