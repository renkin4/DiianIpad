// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Hack/HFloorPlanImage.h"
#include "Components/Image.h"

UHFloorPlanImage::UHFloorPlanImage(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	Index = -1;
	CellInfo = nullptr;
}

void UHFloorPlanImage::Init(int32 CellIndex, FFloorPlanImageInfo* Info)
{
	Index = CellIndex;
	CellInfo = Info;
}

void UHFloorPlanImage::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (ensure(FloorPlanImage))
	{
		if(!CellInfo || !CellInfo->FloorPlanTex) return;

		FloorPlanImage->SetBrushFromTexture(CellInfo->FloorPlanTex, true);
	}
}
