// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UnitDollhouse.h"
#include "Systems/Managers/AVUnitManager.h"
#include "Components/Image.h"
#include "UI/UnitDollhouseHitbox.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

UUnitDollhouse::UUnitDollhouse(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UUnitDollhouse::NativeConstruct()
{
	Super::NativeConstruct();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::TryUnitManager);
}

void UUnitDollhouse::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(GetVisibility() != ESlateVisibility::SelfHitTestInvisible) return;

	float TouchDeltaNormalized = GetDeltaTouchNormalized().X;
	float TouchDelta = GetDeltaTouch().X;
	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Delta : %f, Nromalized : %f"), TouchDelta, TouchDeltaNormalized));
	if (TouchDeltaNormalized != 0.f && FMath::Abs(TouchDelta) > 50.f)
	{
		int32 Sign = FMath::Sign(TouchDeltaNormalized) * -1;
		UnitManager->UpdateDollhouseIndex(Sign);
	}
	
	for(UUnitDollhouseHitbox * It : AllHitboxes)
	{ 
		FVector2D NewPosition;

		GetOwningPlayer()->ProjectWorldLocationToScreen(AllTourPositions[It->Index], NewPosition);

		NewPosition = GetPaintSpaceGeometry().AbsoluteToLocal(NewPosition);

		UCanvasPanelSlot * CSlot = CastChecked<UCanvasPanelSlot >(It->Slot);
		CSlot->SetPosition(NewPosition);
		CSlot->SetSize(FVector2D(32.f, 32.f));
	}
}

void UUnitDollhouse::TryUnitManager()
{
	UnitManager = AAVUnitManager::Get<AAVUnitManager>();

	if(!UnitManager)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::TryUnitManager);
		return;
	}

	OnUnitStateChanges(UnitManager->GetUnitManagerState());
	UnitManager->OnUnitManagerStateChanges.AddDynamic(this, &ThisClass::OnUnitStateChanges);
	UnitManager->OnDollhouseIndexChanges.Add(FIntDelegate::FDelegate::CreateUObject(this, &ThisClass::OnDollHouseIndexChanges));
	UnitManager->OnFinishLoadingDollhouseData.Add(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnFinishLoadingDollhouse));
	UnitManager->OnGFChanges.AddDynamic(this, &ThisClass::OnGFChanges); 
}

void UUnitDollhouse::OnDollHouseIndexChanges(int32 NewIndex)
{
	UTexture2D * Tex = UnitManager->DollhouseTextures[NewIndex];
	SequenceImage->SetBrushFromTexture(Tex, true);
}

void UUnitDollhouse::OnFinishLoadingDollhouse()
{
	UTexture2D * Tex = UnitManager->DollhouseTextures[UnitManager->GetDollhouseIndex()];
	SequenceImage->SetBrushFromTexture(Tex, true);

	InitHitboxes();
}

void UUnitDollhouse::OnUnitStateChanges(EUnitManagerState NewState)
{
	ESlateVisibility NewVisibility = NewState == EUnitManagerState::DOLLHOUSE ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
	SetVisibility(NewVisibility);
}

void UUnitDollhouse::OnGFChanges(EUnitGameplayFeatures NewGF, EUnitGameplayFeatures OldGF)
{
	ESlateVisibility NewVisibility = NewGF == EUnitGameplayFeatures::AERIAL ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
	SetVisibility(NewVisibility);
}

void UUnitDollhouse::InitHitboxes()
{ 
	AllTourPositions.Empty();
	UnitManager->GetCurrentUnitPanoLocations(AllTourPositions);

	if(AllTourPositions.Num() <= 0)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::InitHitboxes); 
		return;
	}

	for (const TPair<int32, FVector> It : AllTourPositions)
	{
		UUnitDollhouseHitbox* HitBox = CreateWidget<UUnitDollhouseHitbox >(this, TourHitboxClass);
		HitBox->Index = It.Key;

		UCanvasPanelSlot* CSlot = DollhouseCanvas->AddChildToCanvas(HitBox);
		CSlot->SetAlignment(FVector2D(0.5f, 0.5f));

		AllHitboxes.Add(HitBox);
	}
}
