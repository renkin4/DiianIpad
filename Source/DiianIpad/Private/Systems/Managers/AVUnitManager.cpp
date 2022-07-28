// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Managers/AVUnitManager.h"
#include "Systems/Utils/AVPanorama.h"
#include "Systems/AVGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/AVCameraMode.h"

AAVUnitManager::AAVUnitManager(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	UnitManagerState = EUnitManagerState::PRIMARY;
	CurrentGF = EUnitGameplayFeatures::AERIAL;

	CurrentBalconyLevel = -1;
	CurrentTourIndex = -1;
	
	DollhouseIndex = 0;

	PrimaryActorTick.bCanEverTick = false;
}

bool AAVUnitManager::GetSelectedUnitData(FUnitData& OutUnitData) const
{ 
	if (!SelectedUnitData) return false;

	OutUnitData = *SelectedUnitData;
	return true;
}

void AAVUnitManager::SetSelectedUnitData(FUnitData Data)
{
	if (SelectedUnitData == &Data || bGettingIntoPers)
	{
		return;
	}

	const FUnitData* PrevData = SelectedUnitData;
	SelectedUnitData = new FUnitData(Data);

	delete PrevData;
	OnSelectedUnitChanges.Broadcast(*SelectedUnitData); 

	if(UnitManagerState == EUnitManagerState::COMPARISON || UnitManagerState == EUnitManagerState::PRIMARY) return;
	SetUnitManagerState(EUnitManagerState::PRIMARY);
}

void AAVUnitManager::SetUnitManagerState(EUnitManagerState NewState)
{
	EUnitManagerState PreviousState = UnitManagerState;

	if (UnitManagerState == NewState)
	{
		return;
	}

	if(NewState == EUnitManagerState::DOLLHOUSE)
	{
		SetCurrentGameplayFeatures(EUnitGameplayFeatures::AERIAL);
		LoadDollhouseData();
		ChooseUnitPano();

		PlayerPawn->Controller->SetControlRotation(FRotator(-50.f, 0.f, 0.f) );
	}

	if(PreviousState == EUnitManagerState::DOLLHOUSE)
	{
		UAVGameInstance* GI = UAVGameInstance::Get();
		for (const TPair<int32, FImageSequence*> Pair : ChoosenDollHouseMap)
		{
			GI->StreamableManager.Unload(Pair.Value->Image.ToSoftObjectPath());
		}

		ChoosenUnitPano.Empty();
		ChoosenDollHouseMap.Empty();
	}

	UnitManagerState = NewState;
	OnUnitManagerStateChanges.Broadcast(UnitManagerState);
}

void AAVUnitManager::SetCurrentGameplayFeatures(EUnitGameplayFeatures NewGF)
{
	if (CurrentGF == NewGF || bGettingInToTour || bGettingIntoPers)
	{
		return;
	}

	EUnitGameplayFeatures PrevGF = CurrentGF;

	if(NewGF == EUnitGameplayFeatures::BALCONY)
	{
		ChangeBalconyLevel(AllBalconyData.Num() - 1);
	}

	if(PrevGF == EUnitGameplayFeatures::BALCONY)
	{
		CurrentBalconyLevel = -1;
	}

	if(NewGF == EUnitGameplayFeatures::TOUR)
	{
		SetupTour();
	}

	if(PrevGF == EUnitGameplayFeatures::TOUR)
	{
		UnloadTour();
	}

	if(NewGF == EUnitGameplayFeatures::AERIAL)
	{
		DollhouseIndex = -1;
		UpdateDollhouseIndex(1);
	}

	CurrentGF = NewGF;
	OnGFChanges.Broadcast(CurrentGF, PrevGF);
}

void AAVUnitManager::GetCurrentUnitPanoLocations(TMap<int32, FVector>& OutMap)
{
	for (const TPair<int32, TSharedPtr<FUnitPanoInfo>> Pair : ChoosenUnitPano)
	{
		OutMap.Add(Pair.Key, Pair.Value->Pano->WorldPos);
	}
}

void AAVUnitManager::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(DT_UnitData))
	{
		AllUnitData.Empty();

		DT_UnitData->GetAllRows<FUnitData>(TEXT(""), AllUnitData);
		SetSelectedUnitData(*AllUnitData[0]);
	}

	if (ensure(DT_BalconyData))
	{
		AllBalconyData.Empty();

		DT_BalconyData->GetAllRows<FBalconyPanoData>(TEXT(""), AllBalconyData);

		for (FBalconyPanoData* Data : AllBalconyData)
		{
			BalconyMap.Add(Data->Index, Data);
		}
	}
	 
	ensure(DT_TypeB_IS);
	ensure(DT_TypeC_IS);

	CheckForPano();
	GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::TryGetPawn);
}

void AAVUnitManager::ChangeBalconyLevel(int32 NewLevel)
{
	if (CurrentBalconyLevel == NewLevel)
	{
		return;
	}

	CurrentBalconyLevel = NewLevel;
	 
	// Aiya won't crash
	FBalconyPanoData* SelectedBalconyData = BalconyMap[CurrentBalconyLevel];

	UTexture * BalconyTex = SelectedBalconyData->Texture.Get();
	if(!BalconyTex)
	{
		BalconyTex = SelectedBalconyData->Texture.LoadSynchronous();
	}

	Pano->SetPanoramaText(BalconyTex);

	OnBalconyChanges.Broadcast(CurrentBalconyLevel);
}

void AAVUnitManager::UpdateDollhouseIndex(int32 Sign /*= 1*/)
{
	DollhouseIndex = ((DollhouseIndex + Sign % ChoosenDollHouseMap.Num()) + ChoosenDollHouseMap.Num()) % ChoosenDollHouseMap.Num();

	float RotateScale = 360.f / ChoosenDollHouseMap.Num();

	float NewYaw = DollhouseIndex * RotateScale;
	
	PlayerPawn->GetController()->SetControlRotation(FRotator(-50.f, NewYaw, 0.f));
	PlayerPawn->SetActorLocation(FVector::ZeroVector);

	OnDollhouseIndexChanges.Broadcast(DollhouseIndex);
}

void AAVUnitManager::ChooseUnitPano()
{
	ChoosenUnitPano.Empty();

	TArray<FUnitPano*> OutArray;
	DT_UnitPano->GetAllRows<FUnitPano>("", OutArray);

	for (FUnitPano* It : OutArray)
	{
		if (SelectedUnitData->Type != It->Type) continue;

		TSharedPtr<FUnitPanoInfo> NewInfo = MakeShared<FUnitPanoInfo>();
		NewInfo->Pano = It;

		ChoosenUnitPano.Add(It->PanoIndex, NewInfo);
	}
}

void AAVUnitManager::LoadDollhouseData()
{ 
	TArray<FImageSequence*> OutArray;

	if(SelectedUnitData->Type == "B")
	{
		DT_TypeB_IS->GetAllRows("", OutArray);
	}
	else
	{
		DT_TypeC_IS->GetAllRows("", OutArray);
	}

	for(FImageSequence * IS: OutArray)
	{
		ChoosenDollHouseMap.Add(IS->Index, IS);

		UTexture2D* Tex = IS->Image.Get();
		if(!Tex)
		{
			Tex = IS->Image.LoadSynchronous();
		}
		DollhouseTextures.Add(IS->Index, Tex);
	}

	OnFinishLoadingDollhouseData.Broadcast();
}

void AAVUnitManager::SetTourIndex(int32 NewTourIndex)
{
	if(CurrentTourIndex == NewTourIndex)
	{
		return;
	}

	CurrentTourIndex = NewTourIndex;
	if(CurrentGF != EUnitGameplayFeatures::TOUR)
	{
		SetCurrentGameplayFeatures(EUnitGameplayFeatures::TOUR);
	}

	OnTourIndexChanges.Broadcast(CurrentTourIndex);

	TSharedPtr<FUnitPanoInfo > Selected = ChoosenUnitPano[CurrentTourIndex];
	Pano->SetPanoramaText(Selected->PanoTex);
	Pano->SetActorLocation(Selected->Pano->WorldPos);
	PlayerPawn->SetActorLocation(Selected->Pano->WorldPos);
}

TSubclassOf<UAVCameraMode> AAVUnitManager::GetCameraMode()
{
	return *CameraModes.Find(CurrentGF); 
}

void AAVUnitManager::GetCurrentLinkedUnitPano(TArray<FUnitPano*>& OutArray)
{
	const TSharedPtr<FUnitPanoInfo> & Info = ChoosenUnitPano[CurrentTourIndex];

	check(DT_UnitPano);

	for (const FName RN : Info->Pano->Links)
	{
		FUnitPano * UPano = DT_UnitPano->FindRow<FUnitPano>(RN, "");

		OutArray.Add(UPano);
	}
}

void AAVUnitManager::SetupTour()
{
	if(!ensure(DT_UnitPano))
	{
		return;
	}

	for (const TPair<int32, TSharedPtr<FUnitPanoInfo>> Pair : ChoosenUnitPano)
	{
		Pair.Value->LoadTexture();
	}
}

void AAVUnitManager::UnloadTour()
{

}

void AAVUnitManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ChoosenUnitPano.Empty();
	Super::EndPlay(EndPlayReason);
}

void AAVUnitManager::CheckForPano()
{
	if (Pano || !ensure(PanoramaClass)) return;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save game states or network managers into a map									

	Pano = GetWorld()->SpawnActor<AAVPanorama>(PanoramaClass, SpawnInfo);
}

void AAVUnitManager::TryGetPawn()
{
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if(!PlayerPawn) 
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::TryGetPawn);
		return;
	}
}

void FUnitPanoInfo::LoadTexture()
{
	check(Pano);

	PanoTex = Pano->PanoTex.Get();

	if(!PanoTex)
	{
		PanoTex = Pano->PanoTex.LoadSynchronous();
	}
}
