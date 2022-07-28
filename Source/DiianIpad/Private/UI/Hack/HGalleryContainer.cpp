// Powered By Yang


#include "UI/Hack/HGalleryContainer.h" 
#include "Systems/Managers/AVGalleryManager.h"
#include "UI/Hack/HGalleryImages.h" 
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h" 
#include "Kismet/KismetSystemLibrary.h"

UHGalleryContainer::UHGalleryContainer(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{ 
	MaxYScale = 700.f;
	CellPaddings = FVector2D(20.f, 0.f);
	ScrollOffset = 0.f;
	ScaleModifier = 1.5f;

	CurrentIndex = -1;
	PreviousIndex = -1;

	BlendTime = 1.f;
	BlendExponent = 4.f;

	CenterInterpSpeed = 20.f;
}

void UHGalleryContainer::NativeConstruct()
{
	Super::NativeConstruct();

	if (ensure(DT_Gallery))
	{
		AllCellData.Empty();
		DT_Gallery->GetAllRows<FGalleryCellData>(TEXT(""), AllCellData);

		LoadTextures();
	}

	SetupManager();
}

void UHGalleryContainer::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UHGalleryContainer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{ 
	Super::NativeTick(MyGeometry, InDeltaTime);

	for (TPair<int32, FCellInfo*>& Pair : CellMap)
	{
		UHGalleryImages* Cell = Pair.Value->Cell;
		if (!Cell) return;

		Cell->CustomTick(InDeltaTime);
	}

	ArrangeChild();
	BlendCellScale(InDeltaTime);
	TryToCenterSelected(InDeltaTime); 
}

void UHGalleryContainer::NativeDestruct()
{ 
	Super::NativeDestruct();

	for (const TPair<int32, FCellInfo*>& Pair : CellMap)
	{
		if (!Pair.Value)
		{
			continue;
		}

		delete Pair.Value;
	}

	CellMap.Empty();
}

FReply UHGalleryContainer::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Manager->SetSelectedIndex(-1);

	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
}

FReply UHGalleryContainer::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	/*if (UAVInactivityComponent::Get())
		UAVInactivityComponent::Get()->ResetTimer();*/

	FVector2D PreviousLoc = CurrentTouchLocation;
	const FReply& SuperReply = Super::NativeOnTouchMoved(InGeometry, InGestureEvent);

	const FVector2D& DeltaTouch = CurrentTouchLocation - PreviousLoc;
	ScrollOffset += DeltaTouch.X;

	return SuperReply;
}

FReply UHGalleryContainer::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	//if (UAVInactivityComponent::Get())
	//	UAVInactivityComponent::Get()->ResetTimer();

	const FReply& SuperReply = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
	const FVector2D& LocalSize = InGeometry.GetLocalSize();

	FCellInfo* ClosestCell = nullptr;
	int32 ClosestIndex = -1;

	for (TPair<int32, FCellInfo*>& Pair : CellMap)
	{
		const FVector2D& Pos = Pair.Value->Slot->GetPosition();
		if (Pos.X >= LocalSize.X || Pair.Key == CurrentIndex) continue;

		if (ClosestCell == nullptr)
		{
			ClosestCell = Pair.Value;
			ClosestIndex = Pair.Key;
			continue;
		}

		const FVector2D& ClosestCellPosition = ClosestCell->Slot->GetPosition();
		if (FMath::Abs(Pos.X) < FMath::Abs(ClosestCellPosition.X))
		{
			ClosestCell = Pair.Value;
			ClosestIndex = Pair.Key;
		}
	}

	Manager->SetSelectedIndex(ClosestIndex);
	return SuperReply;
}

void UHGalleryContainer::LoadTextures()
{ 
	if (AllCellData.Num() <= 0) return;

	for (const FGalleryCellData* Data : AllCellData)
	{
		UTexture2D* Tex = Data->Texture.Get();
		if (!Tex)
		{
			Tex = Data->Texture.LoadSynchronous();
		}

		FCellInfo* NewCell = new FCellInfo(Tex);
		CellMap.Add(Data->Index, NewCell);
	}

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::RefreshSizeNextTick);
}

void UHGalleryContainer::RefreshSizeNextTick()
{
	RecurseRefreshTexture(0);
	InitCells();

	//if (CellMap.Num() > 0)
	//{
	//	ScrollOffset = CellMap[0]->Slot->GetSize().X * 0.5f * -1.f;
	//}
	OnFinishLoading.Broadcast();

}

void UHGalleryContainer::RecurseRefreshTexture(int32 TexIndex)
{
	if (!CellMap.Contains(TexIndex)) return;

	FCellInfo* Info = CellMap[TexIndex];

	bool bSucceeded = Info->RefreshSize();

	if (!bSucceeded)
	{
		FTimerDelegate TimerDel_RecurseRefreshTexture;
		TimerDel_RecurseRefreshTexture.BindUObject(this, &ThisClass::RecurseRefreshTexture, TexIndex);

		GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDel_RecurseRefreshTexture);
		return;
	}

	RecurseRefreshTexture(++TexIndex);
}

void UHGalleryContainer::InitCells()
{
	if (!ensure(GalleryCanvas)) return;

	UClass* GalleryImageClass = GalleryCellClass == nullptr ? UHGalleryImages::StaticClass() : GalleryCellClass.Get();

	for (TPair<int32, FCellInfo*>& Pair : CellMap)
	{
		UHGalleryImages* NewCellWidget = CreateWidget<UHGalleryImages>(GetOwningPlayer(), GalleryImageClass);

		Pair.Value->Cell = NewCellWidget;
		NewCellWidget->Init(Pair.Key, Pair.Value);

		UCanvasPanelSlot* CanvasSlot = GalleryCanvas->AddChildToCanvas(NewCellWidget);
		CellSlotMap.Add(NewCellWidget, CanvasSlot);

		Pair.Value->Slot = CanvasSlot;
		const FVector2D& CellSize = Pair.Value->Size;

		const float FinalScale = CellSize.Y / MaxYScale;
		const FVector2D ScaledSize = CellSize / FinalScale;

		CanvasSlot->SetSize(ScaledSize);
		CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));

		//CanvasSlot->SetZOrder(5);
	}
}

void UHGalleryContainer::ArrangeChild()
{
	AccumulatedSize = ScrollOffset;

	for (TPair<int32, FCellInfo*>& Pair : CellMap)
	{
		UHGalleryImages* Cell = Pair.Value->Cell;
		if (!Cell) continue;
		UCanvasPanelSlot* CSlot = CellSlotMap[Cell];

		const FVector2D& CurrentSize = CSlot->GetSize();

		FVector2D NewPos = FVector2D(AccumulatedSize, 0.f);

		CSlot->SetPosition(NewPos);
		UCanvasPanelSlot* NextCSlot = CellMap.Contains(Pair.Key + 1) ? CellMap[Pair.Key + 1]->Slot : nullptr;
		const float NextSlotSize = NextCSlot ? NextCSlot->GetSize().X * .5f : 0.f;

		AccumulatedSize += (CurrentSize.X * 0.5f) + CellPaddings.X + NextSlotSize;
	}
}

void UHGalleryContainer::SetupManager()
{
	Manager = AAVGalleryManager::Get<AAVGalleryManager>();
	static uint8 RetryTimes = 0;
	if (!Manager)
	{
		ensure(RetryTimes < 5);
		RetryTimes++;
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::SetupManager);
		return;
	}

	Manager->OnSelectedIndexChanged.AddDynamic(this, &ThisClass::OnSelectedIndexChanges);
	Manager->SetSelectedIndex(0);
	/*FTimerHandle TimerHandler;
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ThisClass::Test, 2.0f, false);*/
}

void UHGalleryContainer::TryToCenterSelected(float DeltaTime)
{ 
	if (CurrentIndex < 0 || TouchStartTime > TouchEndTime + 0.2f)
	{
		return;
	}

	FCellInfo* Info = CellMap[CurrentIndex];
	const float Diff = Info->Slot->GetPosition().X * -1;
	const float ConsumeAmount = Diff * DeltaTime * CenterInterpSpeed;

	ScrollOffset += ConsumeAmount;
}

void UHGalleryContainer::BlendCellScale(float DeltaTime)
{
	if (!bStartBlend)
	{
		return;
	}

	float FinalScale = 0.f;
	static FVector2D CurrentCellSize, PreviousCellSize, InitialCurrentCellSize, InitialPreviousCellSize;

	if (bInitBlend)
	{
		if (CurrentIndex >= 0)
		{
			// Aiya wont' crash la
			FCellInfo* CInfo = CellMap[CurrentIndex];
			FinalScale = CInfo->Size.Y / (MaxYScale * ScaleModifier);
			InitialCurrentCellSize = FVector2D(CInfo->Slot->GetSize());
			CurrentCellSize = CInfo->Size / FinalScale;
		}

		if (PreviousIndex >= 0)
		{
			FCellInfo* CInfo = CellMap[PreviousIndex];
			FinalScale = CInfo->Size.Y / (MaxYScale);
			InitialPreviousCellSize = FVector2D(CInfo->Slot->GetSize());
			PreviousCellSize = CInfo->Size / FinalScale;
		}

		bInitBlend = false;
	}

	BlendAlpha += (DeltaTime / BlendTime);
	BlendAlpha = FMath::Min(BlendAlpha, 1.0f);
	const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;
	BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);

	//float DiffToScrollOffset;

	if (CurrentIndex >= 0)
	{
		FCellInfo* CInfo = CellMap[CurrentIndex];
		FVector2D NewSize = FMath::Lerp(InitialCurrentCellSize, CurrentCellSize, BlendWeight);
		CInfo->Slot->SetSize(NewSize);
	}

	if (PreviousIndex >= 0)
	{
		FCellInfo* CInfo = CellMap[PreviousIndex];
		FVector2D NewSize = FMath::Lerp(InitialPreviousCellSize, PreviousCellSize, BlendWeight);

		CInfo->Slot->SetSize(NewSize);
	}

	if (BlendAlpha >= 1)
	{
		OnFinishBlending();
	}
}

void UHGalleryContainer::OnFinishBlending()
{
	bStartBlend = false;
}

void UHGalleryContainer::OnSelectedIndexChanges(int32 NewIndex)
{

	PreviousIndex = CurrentIndex;
	CurrentIndex = NewIndex;

	bInitBlend = true;
	bStartBlend = true;
	BlendAlpha = 0.f;

	K2_OnSelectedIndexChanges(NewIndex);
}
