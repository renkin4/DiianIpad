// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Managers/AVManagerBase.h"
#include "Data/AVType.h"
#include "AVUnitManager.generated.h"

class AAVPanorama;
class UAVCameraMode;
class UMaterialInstanceDynamic;

UENUM(BlueprintType)
enum class EUnitManagerState : uint8
{
	NONE = 0 UMETA(Hidden),
	PRIMARY,
	COMPARISON,
	DOLLHOUSE,
	GROUND,
	STOREY,
};

struct FUnitPanoInfo 
{

public:
	FUnitPanoInfo()
		:Pano(nullptr),
		PanoTex(nullptr)
	{} 

	FUnitPano * Pano;
	UTexture * PanoTex;

public:
	void LoadTexture();

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectUnitDataSignature, const FUnitData&, NewData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUnitManagerStateSignature, EUnitManagerState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUnitGameplayFeaturesSignature, EUnitGameplayFeatures, NewGF, EUnitGameplayFeatures, OldGF);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBalconySignature, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimpleAVDelegate);

/**
 * 
 */
UCLASS()
class DIIANIPAD_API AAVUnitManager : public AAVManagerBase
{
	GENERATED_BODY()

public:
	AAVUnitManager(const class FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintAssignable, Category = "AV Unit")
	FSelectUnitDataSignature OnSelectedUnitChanges;

	UPROPERTY(BlueprintAssignable, Category = "AV Unit")
	FUnitManagerStateSignature OnUnitManagerStateChanges;

	UPROPERTY(BlueprintAssignable, Category = "AV Unit")
	FUnitGameplayFeaturesSignature OnGFChanges;

	UPROPERTY(BlueprintAssignable, Category = "AV Unit")
	FBalconySignature OnBalconyChanges;

	UPROPERTY(BlueprintAssignable, Category = "AV Unit")
	FBalconySignature OnTourIndexChanges; // AIyayaya

	FSimpleMulticastDelegate OnBalconySelected;
	FIntDelegate OnDollhouseIndexChanges;
	FSimpleMulticastDelegate OnFinishLoadingDollhouseData;

public: 
	UFUNCTION(BlueprintPure, Category="AV Unit")
	bool GetSelectedUnitData(FUnitData & OutUnitData) const;

	UFUNCTION(BlueprintCallable, Category="AV Unit")
	void SetSelectedUnitData(FUnitData Data);
	
	UFUNCTION(BlueprintPure, Category = "AV Unit")
	int32 GetAmountOfBalconyPano() const { return BalconyMap.Num(); }

	UFUNCTION(BlueprintPure, Category = "AV Unit")
	EUnitManagerState GetUnitManagerState() const { return UnitManagerState; }
	
	UFUNCTION(BlueprintCallable, Category = "AV Unit")
	void SetUnitManagerState(EUnitManagerState NewState);
	
	UFUNCTION(BlueprintCallable, Category = "AV Unit")
	void SetCurrentGameplayFeatures(EUnitGameplayFeatures NewGF);

	UFUNCTION(BlueprintCallable, Category = "AV Unit")
	void GetCurrentUnitPanoLocations(TMap<int32, FVector> & OutMap);

	EUnitGameplayFeatures GetCurrentGF() const { return CurrentGF; }
	
	//~ Begin 
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End
	
	UFUNCTION(BlueprintCallable, Category = "Unit")
	void ChangeBalconyLevel(int32 NewLevel);

	void UpdateDollhouseIndex(int32 Sign = 1);

	FImageSequence* ChoosenDollhouseData() const { return ChoosenDollHouseMap[DollhouseIndex]; }
	
	void ChooseUnitPano();

	void LoadDollhouseData();
	int32 GetDollhouseIndex() const { return DollhouseIndex; }

	UFUNCTION(BlueprintCallable, Category = "Unit")
	void SetTourIndex(int32 NewTourIndex);

	TSubclassOf<UAVCameraMode> GetCameraMode();

	void GetCurrentLinkedUnitPano(TArray<FUnitPano*> & OutArray);

public:
	TMap<int32, UTexture2D*> DollhouseTextures;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
	UDataTable * DT_UnitData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
	UDataTable* DT_BalconyData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
	UDataTable* DT_TypeB_IS;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
	UDataTable* DT_TypeC_IS;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
	UDataTable* DT_UnitPano;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
	EUnitGameplayFeatures CurrentGF;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
	TMap<EUnitGameplayFeatures, TSubclassOf<UAVCameraMode>> CameraModes;

	const FUnitData * SelectedUnitData;
	
	void SetupTour();
	void UnloadTour(); 

private:
	void CheckForPano(); 
	void TryGetPawn(); 

private:
	EUnitManagerState UnitManagerState;

	TArray<FUnitData*> AllUnitData;
	TArray<FBalconyPanoData*> AllBalconyData;

	TMap<int32, FBalconyPanoData*> BalconyMap;
	TMap<int32, FImageSequence*> ChoosenDollHouseMap; 
	TMap<int32, TSharedPtr<FUnitPanoInfo>> ChoosenUnitPano;

	UPROPERTY(Transient)
	TArray<AActor*> AllCameras; // perpsective cameras 

	UPROPERTY(Transient)
	TArray<AActor*> AllUnitActors;

	UPROPERTY(Transient)
	TSoftObjectPtr<UWorld> CurrentDollHouse;

	uint8 bGettingInToTour : 1; 
	uint8 bGettingIntoPers : 1;
	 
	int32 CurrentBalconyLevel;
	int32 CurrentTourIndex;

	UMaterialInstanceDynamic* PanoMID;
	
	UPROPERTY(EditAnywhere, Category = "AV")
	TSubclassOf<AAVPanorama> PanoramaClass;

	UPROPERTY(Transient)
	AAVPanorama* Pano;

	UPROPERTY(Transient)
	APawn * PlayerPawn;

	int32 DollhouseIndex;
};
