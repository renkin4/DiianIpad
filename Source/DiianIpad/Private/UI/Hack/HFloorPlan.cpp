// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Hack/HFloorPlan.h"
#include "Systems/Managers/AVUnitManager.h"
#include "Engine/StreamableManager.h"
#include "Systems/AVGameInstance.h"
#include "UI/Hack/HFloorPlanImage.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UHFloorPlan::UHFloorPlan(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	MaxYScale = 650.f;
	CellPaddings = FVector2D(20.f, 0.f);
	
	ScrollOffset = 0.f;
	AccumulatedSize = 0.f;

	CenterInterpSpeed = 20.f;

	CurrentIndex = -1;
	Direction = 0;

	bCustomUnitData = false;
}

void UHFloorPlan::NativeConstruct()
{
	Super::NativeConstruct();

	SetupManager(); 
}

void UHFloorPlan::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (ensure(DT_UnitData))
	{
		TArray<FUnitData * > OutArray;

		AllFloorPlansInfo.Empty();
		DT_UnitData->GetAllRows<FUnitData>(TEXT(""), OutArray);

		int32 i = 0;
		for(FUnitData * It : OutArray)
		{
			FFloorPlanImageInfo * NewInfo = new FFloorPlanImageInfo(It);
			AllFloorPlansInfo.Add(i, NewInfo);

			NewInfo->LoadUnit();
			i++;
		}

		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::RefreshTextureSizes);
	}

}

void UHFloorPlan::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ArrangeChild();
	TryToCenterSelected(InDeltaTime);
}

void UHFloorPlan::NativeDestruct()
{
	Super::NativeDestruct();

	for (const TPair<int32, FFloorPlanImageInfo*> Pair : AllFloorPlansInfo)
	{
		if(!Pair.Value)  continue;

		delete Pair.Value;
	}
}

FReply UHFloorPlan::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{ 
	FVector2D PreviousLoc = CurrentTouchLocation;
	const FReply& SuperReply = Super::NativeOnTouchMoved(InGeometry, InGestureEvent);

	const FVector2D& DeltaTouch = CurrentTouchLocation - PreviousLoc;
	ScrollOffset += DeltaTouch.X;

	Direction = FMath::Sign(DeltaTouch.X);

	return SuperReply; 

}

FReply UHFloorPlan::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	const FReply& SuperReply = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	if(!UnitManager)
	{
		return SuperReply;
	}

	int32 NewIndex = FMath::Clamp(CurrentIndex + (Direction * -1), 0, AllFloorPlansInfo.Num() - 1);
	FUnitData Data = *AllFloorPlansInfo[NewIndex]->UnitData;

	//UnitManager->SetSelectedUnitData(Data);
	K2_OnSelectedUnitChanges.Broadcast(Data);

	return SuperReply;
}

void UHFloorPlan::RefreshTextureSizes()
{
	for(const TPair<int32, FFloorPlanImageInfo*> Pair: AllFloorPlansInfo)
	{
		Pair.Value->RefreshSize();
	}

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::InitCells);
}

void UHFloorPlan::InitCells()
{
	if(!ensure(FloorPlanCanvas) || !ensure(FloorPlanImageClass)) return;

	for (const TPair<int32, FFloorPlanImageInfo*> Pair : AllFloorPlansInfo)
	{ 
		UHFloorPlanImage * NewFloorPlan = CreateWidget<UHFloorPlanImage>(this, FloorPlanImageClass);
		if(!NewFloorPlan) continue;

		Pair.Value->FloorPlanImage = NewFloorPlan;
		NewFloorPlan->Init(Pair.Key, Pair.Value);

		UCanvasPanelSlot* CSlot = FloorPlanCanvas->AddChildToCanvas(NewFloorPlan);
		
		Pair.Value->Slot = CSlot;
		const FVector2D& CellSize = Pair.Value->Size;

		const float FinalScale = CellSize.Y / MaxYScale;
		const FVector2D ScaledSize = CellSize / FinalScale;

		CSlot->SetSize(ScaledSize);
		CSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		CSlot->SetAnchors(FAnchors(0.5f, 0.5f));
	}

	ArrangeChild();
}

void UHFloorPlan::ArrangeChild()
{
	AccumulatedSize = ScrollOffset;

	for (TPair<int32, FFloorPlanImageInfo*>& Pair : AllFloorPlansInfo)
	{
		if (!Pair.Value->FloorPlanImage || !Pair.Value->Slot) continue;

		UCanvasPanelSlot* CSlot = Pair.Value->Slot;

		const FVector2D& CurrentSize = CSlot->GetSize();

		FVector2D NewPos = FVector2D(AccumulatedSize, 0.f);

		CSlot->SetPosition(NewPos);
		UCanvasPanelSlot* NextCSlot = AllFloorPlansInfo.Contains(Pair.Key + 1) ? AllFloorPlansInfo[Pair.Key + 1]->Slot : nullptr;
		const float NextSlotSize = NextCSlot ? NextCSlot->GetSize().X * .5f : 0.f;

		AccumulatedSize += (CurrentSize.X * 0.5f) + CellPaddings.X + NextSlotSize;
	}
}

void UHFloorPlan::SetupManager()
{
	UnitManager = AAVUnitManager::Get<AAVUnitManager>();
	static uint8 RetryTimes = 0;
	if (!UnitManager)
	{
		ensure(RetryTimes < 5);
		RetryTimes++;
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::SetupManager);
		return;
	}

	if(bCustomUnitData)
	{
		return;
	}

	UnitManager->OnSelectedUnitChanges.AddDynamic(this, &ThisClass::OnSelectedUnitChanges);

	FUnitData OutData;
	UnitManager->GetSelectedUnitData(OutData);
	OnSelectedUnitChanges(OutData);
}

void UHFloorPlan::TryToCenterSelected(float DeltaTime)
{
	if (CurrentIndex < 0 || TouchStartTime > TouchEndTime + 0.2f)
	{
		return;
	}

	FFloorPlanImageInfo* Info = AllFloorPlansInfo[CurrentIndex];

	if(!AllFloorPlansInfo[CurrentIndex]->Slot) return;

	const float Diff = (Info->Slot->GetPosition().X + -85.f) * -1;
	const float ConsumeAmount = Diff * DeltaTime * CenterInterpSpeed;

	ScrollOffset += ConsumeAmount;
}

void UHFloorPlan::OnSelectedUnitChanges(const FUnitData & UnitData)
{  
	for (TPair<int32, FFloorPlanImageInfo*>& Pair : AllFloorPlansInfo)
	{
		const FUnitData & Data = *Pair.Value->UnitData;
		if (UnitData == Data) 
		{
			CurrentIndex = Pair.Key;
			break;
		}
	}

	FFloorPlanImageInfo * Choosen = AllFloorPlansInfo[CurrentIndex];
	if(!Choosen)
	{
		return;
	}

	check(TypeText && TypeTextDescription);

	FUnitData * ChoosenData = Choosen->UnitData;

	FString TypeTextString = FString::Printf(TEXT("Type %s"), *ChoosenData->Type);
	FString TypeDescriptionString = FString::Printf(TEXT("%d SQ.FT | %d Bedrooms + %d Bathroom"), (int32)ChoosenData->SqFt, ChoosenData->Bedroom, ChoosenData->Toilet);

	TypeText->SetText(FText::FromString(TypeTextString));
	TypeTextDescription->SetText(FText::FromString(TypeDescriptionString));

	//const float MaxX = 350.0f;
	////ImageType->SetBrushFromTexture(Choosen->TypeTex, false);

	//FVector2D TypeSize = FVector2D(Choosen->TypeTex->GetSizeX(), Choosen->TypeTex->GetSizeY());

	//const float TypeFinalScale = Choosen->TypeTex->GetSizeX() / MaxX;
	//const FVector2D NewSize = TypeSize / TypeFinalScale;

	//ImageType->SetBrushSize(NewSize);
}

void UHFloorPlan::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if(TouchStartTime <= TouchEndTime) return;

	TouchEndTime = GetWorld()->TimeSeconds;

	int32 NewIndex = FMath::Clamp(CurrentIndex + (Direction * -1), 0, AllFloorPlansInfo.Num() - 1);
	FUnitData Data = *AllFloorPlansInfo[NewIndex]->UnitData;

	//UnitManager->SetSelectedUnitData(Data);
	K2_OnSelectedUnitChanges.Broadcast(Data);
}

void FFloorPlanImageInfo::LoadUnit()
{
	if(!ensure(UnitData)) return;

	FloorPlanTex = UnitData->FloorPlan.Get();  

	if(!FloorPlanTex)
	{
		FloorPlanTex = UnitData->FloorPlan.LoadSynchronous();
	} 
}

void FFloorPlanImageInfo::RefreshSize()
{
	check(FloorPlanTex);

	Size = FVector2D(FloorPlanTex->GetSizeX(), FloorPlanTex->GetSizeY());
	AspectRatio = (float)Size.X / (float)Size.Y;
}
