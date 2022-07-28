// Powered By Yang

#include "Systems/Managers/AVPodiumManager.h" 
#include "Systems/Utils/AVPanorama.h"
#include "DatasmithAssetUserData.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/PostProcessVolume.h"

static FString sPodiumID = TEXT("PodiumID");
static FString sVRID = TEXT("VRID");

AAVPodiumManager::AAVPodiumManager(const class FObjectInitializer& ObjectInitializer)	
	:Super(ObjectInitializer)
{
	CurrentState = EPodiumState::SEQUENCER;
	CurrentIndex = -1;

	bFinishLoadingTexture = false;
	bGround = false;

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AAVPodiumManager::SetState(EPodiumState NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	EPodiumState PrevState = CurrentState;

	CurrentState = NewState;
	OnPodiumStateChanges.Broadcast(CurrentState, PrevState);

	if (PrevState == EPodiumState::SEQUENCER)
	{
		FRotator Rot;
		if (CurrentState == EPodiumState::VR)
		{
			//~ HAck =)
			const TMap<FString, AActor* >& Dius = DiuMaps[sVRID];
			AActor* Diu = Dius[CurrentVR.ToString()];
			ensure(Diu);

			AActor* Diu11 = Dius[TEXT("11")];

			FVector Dir = Diu11->GetActorLocation() - Diu->GetActorLocation();
			Rot = FRotationMatrix::MakeFromX(Dir).Rotator();

		}

		if (CurrentState == EPodiumState::GROUND)
		{
			Rot = FRotator(0.f, -90.f, 0.f); // WHo cares about copy constructor? Does anyone knows?
		}

		PlayerPawn->Controller->SetControlRotation(Rot);
	}

	//if(CurrentState == EPodiumState::GROUND)
	//{
	//	PP->Settings.AutoExposureBias = 11.f;
	//}
	//else if(CurrentState == EPodiumState::VR)
	//{
	//	PP->Settings.AutoExposureBias = 8.5f;
	//}

	if (PrevState == EPodiumState::GROUND)
	{
		bGround = false;
	}

	if (PrevState == EPodiumState::GROUND || PrevState == EPodiumState::VR)
	{
		CurrentVR = NAME_None;
	}
}

void AAVPodiumManager::UpdateCurrentIndex(int32 Sign)
{ 
	int32 NewVal = CurrentIndex + Sign;
	const int32 MaxVal = AllImageSequence.Num() - 2;

	NewVal = FMath::Clamp(NewVal, 0, MaxVal);

	ChangeIndex(NewVal); 
}

void AAVPodiumManager::ChangeIndex(int32 NewIndex)
{ 
	if (NewIndex == CurrentIndex)
	{
		return;
	}

	int32 OldIndex = CurrentIndex;

	CurrentIndex = NewIndex;
	OnPodiumIndexChages.Broadcast(CurrentIndex, OldIndex);
	DeterminePawnPosition();
}

TSubclassOf<UAVCameraMode> AAVPodiumManager::GetCurrentCameraMode() const
{
	return PodiumCameraMode[CurrentState];

}

void AAVPodiumManager::SetVR(const FName& VR)
{
	if (CurrentVR == VR)
	{
		return;
	}

	CurrentVR = VR;

	if (bGround)
	{
		if (CurrentState != EPodiumState::GROUND)
		{
			SetState(EPodiumState::GROUND);
		}
	}
	else if (CurrentState != EPodiumState::VR)
	{
		SetState(EPodiumState::VR);
	}

	CheckForPano();

	FPodiumData* FoundData = DT_PodiumData->FindRow<FPodiumData>(CurrentVR, TEXT(""));
	ensure(FoundData);

	//~ HAck =)
	const TMap<FString, AActor* >& Dius = DiuMaps[sVRID];
	AActor* Diu = Dius[VR.ToString()];
	Pano->SetActorLocation(Diu->GetActorLocation());

	SetVRByData(*FoundData);
}

void AAVPodiumManager::SetVRByData(const FPodiumData& Data)
{ 
	CurrentVRData = const_cast<FPodiumData*>(&Data);
	Pano->SetPanoData(Data);

	PlayerPawn->SetActorLocation(Pano->GetActorLocation());

	OnPanoVRChanges.Broadcast(Data);
}

void AAVPodiumManager::SetGround()
{
	const FName VR = TEXT("15");
	bGround = true;
	SetVR(VR);
}

TArray<FPodiumData> AAVPodiumManager::GetCurrentLinksData()
{
	ensure(CurrentVRData);
	TArray<FPodiumData> ReturnData;
	//UDataTable * DataTable = CurrentState == EPodiumState::VR ? DT_PodiumData : DT_PodiumGroundData;
	UDataTable* DataTable = DT_PodiumData;

	ensure(DataTable);

	for (int32 i = 0; i < CurrentVRData->Links.Num(); ++i)
	{
		const FName& RowName = CurrentVRData->Links[i];

		FPodiumData* FoundData = DataTable->FindRow<FPodiumData>(RowName, TEXT(""));

		ReturnData.Add(*FoundData);
	}

	return ReturnData;
}

void AAVPodiumManager::GetCurrentLink(TArray<FName>& OutLink)
{
	OutLink = CurrentVRData->Links;

}

void AAVPodiumManager::GetCurrentLinkVRDiu(TArray<AActor*>& OutVR)
{
	TMap<FString, AActor*>& ChoosenMap = DiuMaps[sVRID];

	for (int32 i = 0; i < CurrentVRData->Links.Num(); ++i)
	{
		const FString& LinkString = CurrentVRData->Links[i].ToString();
		OutVR.Add(ChoosenMap[LinkString]);
	}
}

void AAVPodiumManager::GetPodiumVRDiu(TMap<FString, AActor*>& OutVR)
{
	OutVR = DiuMaps[sPodiumID];

}

void AAVPodiumManager::GetVRDiu(TMap<FString, AActor*>& OutVR)
{
	OutVR = DiuMaps[sVRID];

}

AActor* AAVPodiumManager::GetVRDiuFromName(const FName& Key)
{

	TMap<FString, AActor*>& ChoosenMap = DiuMaps[sVRID];

	return ChoosenMap[Key.ToString()];
}

void AAVPodiumManager::GetAllDiu(TArray<AActor*>& OutDiu)
{
	OutDiu = AllDius;
}

void AAVPodiumManager::BeginPlay()
{
	Super::BeginPlay();
	if (ensure(DT_PodiumData))
	{
		AllPodiumData.Empty();

		DT_PodiumData->GetAllRows<FPodiumData>(TEXT(""), AllPodiumData);
	}

	if (ensure(DT_PodiumIS))
	{
		AllImageSequence.Empty();

		DT_PodiumIS->GetAllRows<FImageSequence>(TEXT(""), AllImageSequence);
		LoadImages();
	}

	TryGetPawn();
	GetDius();
	TryGetPP();
}

void AAVPodiumManager::LoadImages()
{
	for (const FImageSequence* IS : AllImageSequence)
	{
		ensure(IS);
		UTexture2D* Tex = IS->Image.LoadSynchronous();

		LoadedImages.Add(IS->Index, Tex);
	}

	bFinishLoadingTexture = true;
	OnFinishLoadingTextures.Broadcast();
}

void AAVPodiumManager::DeterminePawnPosition()
{
	if (CurrentState == EPodiumState::SEQUENCER)
	{
		// Too lazy to check if it exist. If not just crash it.
		FTransform& NewTransform = StoredTransform[CurrentIndex * 6];

		PlayerPawn->SetActorLocation(NewTransform.GetLocation());
		PlayerPawn->Controller->SetControlRotation(NewTransform.GetRotation().Rotator());
	}
}

void AAVPodiumManager::TryGetPawn()
{
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if (!PlayerPawn)
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::TryGetPawn);
	}
}

void AAVPodiumManager::CheckForPano()
{
	if (Pano) return;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save game states or network managers into a map									

	Pano = GetWorld()->SpawnActor<AAVPanorama>(PanoramaClass, SpawnInfo);
}

void AAVPodiumManager::GetDius()
{
	AllDius.Empty();
	UGameplayStatics::GetAllActorsWithTag(this, TEXT("Diu"), AllDius);

	for (AActor* Diu : AllDius)
	{
		UDatasmithAssetUserData* DataSmithData = UDatasmithAssetUserData::GetDatasmithUserData(Diu);
		//Diu->GetDatasmith
		bool bPodiumDiu = DataSmithData->MetaData.Contains(FName(sPodiumID));
		if (bPodiumDiu)
		{
			TMap<FString, AActor*>& PodiumDius = DiuMaps.FindOrAdd(sPodiumID);
			PodiumDius.Add(DataSmithData->MetaData[FName(sPodiumID)], Diu);
		}

		// hard code VRID because 100% All of them have it
		TMap<FString, AActor*>& VRDius = DiuMaps.FindOrAdd(sVRID);
		VRDius.Add(DataSmithData->MetaData[FName(sVRID)], Diu);
	}
	 
}

void AAVPodiumManager::TryGetPP()
{
	TArray<AActor*> OutPP;
	UGameplayStatics::GetAllActorsWithTag(this, TEXT("PP"), OutPP);

	if(OutPP.Num() <= 0) 
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::TryGetPP);
		return;
	}

	PP = CastChecked<APostProcessVolume>(OutPP[0]);
}
