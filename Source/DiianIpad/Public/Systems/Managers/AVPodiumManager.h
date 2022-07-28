// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "Systems/Managers/AVManagerBase.h"
#include "Data/AVType.h"
#include "AVPodiumManager.generated.h"

class UAVCameraMode;
class UDataTable;
class AAVPanorama;

UENUM(BlueprintType)
enum class EPodiumState : uint8
{
	NONE = 0 UMETA(Hidden),
	SEQUENCER,
	VR,
	GROUND,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPodiumStateSignature, EPodiumState, NewState, EPodiumState, OldState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPodiumIndexSignature, int32, NewIndex, int32, OldIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFinishLoadingSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPanoSignature, FPodiumData, Data);

/**
 * 
 */
UCLASS()
class DIIANIPAD_API AAVPodiumManager : public AAVManagerBase
{
	GENERATED_BODY()
	
public:
	AAVPodiumManager(const class FObjectInitializer & ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category = "AV Podium")
	void SetState(EPodiumState NewState);

	UFUNCTION(BlueprintCallable, Category = "AV Podium")
	void UpdateCurrentIndex(int32 Sign);

	UFUNCTION(BlueprintCallable, Category = "AV Podium")
	void ChangeIndex(int32 NewIndex);

	UPROPERTY(BlueprintAssignable, Category="AV Podium")
	FPodiumStateSignature OnPodiumStateChanges;

	UPROPERTY(BlueprintAssignable, Category = "AV Podium")
	FPodiumIndexSignature OnPodiumIndexChages;

	UPROPERTY(BlueprintAssignable, Category = "AV Podium")
	FFinishLoadingSignature OnFinishLoadingTextures;

	UPROPERTY(BlueprintAssignable, Category = "AV Podium")
	FPanoSignature OnPanoVRChanges;

	TSubclassOf<UAVCameraMode> GetCurrentCameraMode() const;

	UFUNCTION(BlueprintCallable, Category = "AV Podium")
	void SetVR(const FName& VR);

	UFUNCTION(BlueprintCallable, Category = "AV Podium")
	void SetVRByData(const FPodiumData & Data);

	UFUNCTION(BlueprintCallable, Category = "AV Podium")
	void SetGround();

	UFUNCTION(BlueprintPure, Category = "AV Podium")
	TArray<FPodiumData> GetCurrentLinksData();
	
	UFUNCTION(BlueprintPure, Category = "AV Podium")
	void GetCurrentLink(TArray<FName> & OutLink);

	UFUNCTION(BlueprintPure, Category = "AV Podium")
	void GetCurrentLinkVRDiu(TArray<AActor*> & OutVR);

	UFUNCTION(BlueprintPure, Category = "AV Podium")
	void GetPodiumVRDiu(TMap<FString, AActor*> & OutVR);

	UFUNCTION(BlueprintPure, Category = "AV Podium")
	void GetVRDiu(TMap<FString, AActor*> & OutVR);

	UFUNCTION(BlueprintPure, Category = "AV Podium")
	AActor* GetVRDiuFromName(const FName & Key);
	
	void GetAllDiu(TArray<AActor*> & OutDiu);

	bool GetFinishLoadingTextures() const { return bFinishLoadingTexture; }

	UPROPERTY(BlueprintReadOnly, Category = "AV Podium")
	TMap<int32, UTexture2D*> LoadedImages;

protected:
	//~ Begin AActor Override
	virtual void BeginPlay() override;
	//~ End AActor Override

	void LoadImages();
	void DeterminePawnPosition();
	void TryGetPawn();
	void CheckForPano();
	void GetDius();
	void TryGetPP();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AV Podium")
	EPodiumState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AV Podium")
	TMap<EPodiumState, TSubclassOf<UAVCameraMode>> PodiumCameraMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AV Podium")
	TArray<FTransform> StoredTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AV Podium")
	int32 CurrentIndex;
	 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AV Podium")
	UDataTable * DT_PodiumData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AV Podium")
	UDataTable * DT_PodiumGroundData;

	// Image Sequence
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AV Podium")
	UDataTable * DT_PodiumIS;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AV Podium")
	TSubclassOf<AAVPanorama> PanoramaClass;

	UPROPERTY(BlueprintReadOnly, Category = "AV Podium")
	uint8 bFinishLoadingTexture : 1;

	UPROPERTY(Transient)
	class APostProcessVolume * PP;

private:
	TArray<FPodiumData*> AllPodiumData;
	TArray<FImageSequence*> AllImageSequence; 
	APawn * PlayerPawn; 

	FName CurrentVR;
	FPodiumData* CurrentVRData;

	UPROPERTY(Transient)
	AAVPanorama * Pano;

	uint8 bGround : 1;

	TArray<AActor *> AllDius;
	TMap<FString, TMap<FString, AActor*>> DiuMaps;
};
