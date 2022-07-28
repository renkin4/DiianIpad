// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "Surrounding/AVSurroundingBase.h"
#include "AVSurroundingGeneral.generated.h"

/**
 * 
 */
UCLASS()
class DIIANIPAD_API AAVSurroundingGeneral : public AAVSurroundingBase
{
	GENERATED_BODY()

public:
	AAVSurroundingGeneral(const class FObjectInitializer & ObjectInitializer);

protected:
	//~ Begin Override 
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual FString GetPOIName_Implementation() override;
	
	//~ End Override 

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AV")
	USceneComponent* SceneRootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AV")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV", meta=(DisplayPriority =1))
	FString POIName;

protected:
	void TryGetPC();
	
	UFUNCTION(BlueprintPure, Category = "AV")
	static APlayerController* GetCachePC();

private:
	static APlayerController* s_CachePlayerController;
};
