// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UnitVR.h"
#include "Systems/Managers/AVUnitManager.h"
#include "UI/UnitDollhouseHitbox.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UUnitVR::NativeConstruct()
{
	Super::NativeConstruct();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::TryGetUnitManager);
}

void UUnitVR::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime); 

	for(UUnitDollhouseHitbox * HB : AllHitboxes)
	{
		FVector2D NewPos;
		bool bInScreen = GetOwningPlayer()->ProjectWorldLocationToScreen(HitboxMap[HB->Index], NewPos);

		NewPos = GetPaintSpaceGeometry().AbsoluteToLocal(NewPos);

		HB->SetVisibility(bInScreen ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

		UCanvasPanelSlot * CSlot = Cast<UCanvasPanelSlot>(HB->Slot);
		CSlot->SetPosition(NewPos);
	}
}

void UUnitVR::TryGetUnitManager()
{
	UnitManager = AAVUnitManager::Get<AAVUnitManager>();
	if(!UnitManager)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::TryGetUnitManager);
		return;
	}

	UnitManager->OnGFChanges.AddDynamic(this, &ThisClass::OnGFChanges);
	UnitManager->OnTourIndexChanges.AddDynamic(this, &ThisClass::OnTourIndexChanges);
}

void UUnitVR::InitHitbox()
{
	if(!ensure(DollHouseHitboxClass)) return;
	
	for (UUnitDollhouseHitbox* It : AllHitboxes)
	{
		It->RemoveFromParent();
	}

	AllHitboxes.Empty();
	HitboxMap.Empty();

	TArray<FUnitPano*> Linked;
	UnitManager->GetCurrentLinkedUnitPano(Linked);

	for(FUnitPano * L : Linked)
	{
		HitboxMap.Add(L->PanoIndex, L->WorldPos);

		UUnitDollhouseHitbox * HB = CreateWidget<UUnitDollhouseHitbox>(this, DollHouseHitboxClass); 
		HB->Index = L->PanoIndex;

		UCanvasPanelSlot* CSlot = UnitVRCanvas->AddChildToCanvas(HB);
		CSlot->SetAlignment(FVector2D(.5f, .5f) );
		CSlot->SetSize(FVector2D(64.f, 64.f));

		AllHitboxes.Add(HB);
	}
}

void UUnitVR::OnGFChanges(EUnitGameplayFeatures NewGF, EUnitGameplayFeatures OldGF)
{
	ESlateVisibility NewVisibility = NewGF == EUnitGameplayFeatures::TOUR ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;

	SetVisibility(NewVisibility);
}

void UUnitVR::OnStateChanges(EUnitManagerState NewState)
{
	ESlateVisibility NewVisibility = NewState == EUnitManagerState ::DOLLHOUSE ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;

	SetVisibility(NewVisibility);
}

void UUnitVR::OnTourIndexChanges(int32 Index)
{
	CurrentIndex = Index;

	InitHitbox();
}
