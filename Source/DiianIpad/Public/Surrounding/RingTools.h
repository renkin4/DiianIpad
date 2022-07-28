// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "Surrounding/AVSurroundingBase.h"
#include "RingTools.generated.h"

class UProceduralMeshComponent;
class UTextRenderComponent;
class UMaterialInstanceDynamic;

/**
 * 
 */
UCLASS()
class DIIANIPAD_API ARingTools : public AAVSurroundingBase
{
	GENERATED_BODY()
	
public:
	ARingTools(const class FObjectInitializer& ObjectInitializer);

public:	
	static FName ProceduralMeshName;
	static FName TextRendererName;
	 
	 //
	virtual FString GetPOIName_Implementation() override;
	 //

	 float GetRadius() const { return Radius; }

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Mesh", meta = (AllowPrivateAccess = true))
	UProceduralMeshComponent* ProceduralMeshComp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Text", meta = (AllowPrivateAccess = true))
	UTextRenderComponent* TextRendererComp;
	 
protected:
	//~ Begin AActor override 
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSec) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	virtual void OnFinishedRegistering() override;
	//~ End of AActor override

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Ring Tool", meta = (DevelopmentOnly))
	void PivotToFacade();

	void UpdateTargetRotation(float DeltaSec);
	void GetAllPOIRadiusNextTick();

	void TryGetPC();
	void HackPOI(bool bNewHidden);

	UFUNCTION(BlueprintPure , Category = "AV")
	UMaterialInstanceDynamic* GetMID() const { return MID; }

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ring Tool", meta = (DisplayPriority = 1))
	int32 NoOfSegments;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ring Tool", meta = (DisplayPriority = 1))
	float Radius;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ring Tool", meta = (DisplayPriority = 1))
	FLinearColor RingColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ring Tool", meta = (DisplayPriority = 1))
	UMaterialInterface* MaterialInterface;

	/** Max time step used when sub-stepping lag. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ring Tool", AdvancedDisplay, meta = (ClampMin = "0.005", ClampMax = "0.5", UIMin = "0.005", UIMax = "0.5"))
	float LagMaxTimeStep;

	/** controls how quickly reaches target position. Low values are slower (more lag), high values are faster (less lag), while zero is instant (no lag). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ring Tool", meta = (ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"))
	float RotationLagSpeed;

private:
	UMaterialInstanceDynamic* MID;

	FRotator TargetRotation;
	/** Temporary variable for lagging camera rotation, for previous rotation */
	FRotator PreviousTargetRot;
	
	TSet<AActor*> AllPOIRadius;

	UPROPERTY(Transient)
	class APlayerController * SurroundingPC;
};
