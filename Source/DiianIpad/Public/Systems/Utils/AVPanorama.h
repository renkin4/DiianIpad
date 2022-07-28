// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/AVType.h"
#include "AVPanorama.generated.h"

UCLASS()
class DIIANIPAD_API AAVPanorama : public AActor
{
	GENERATED_BODY()
	
public:	
	AAVPanorama(const class FObjectInitializer & ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "AV Panorama")
	virtual void SetPanoramaText(const TSoftObjectPtr<UTexture>& PanoTexture);

	UFUNCTION(BlueprintCallable, Category = "AV Panorama")
	void SetPanoData(const FPodiumData & PanoData);

protected:
	UPROPERTY(VisibleAnywhere, Category = "AV Panorama")
	USceneComponent * PanoRoot;

	UPROPERTY(VisibleAnywhere, Category = "AV Panorama")
	UStaticMeshComponent * SM;

	UPROPERTY(Transient)
	class UMaterialInstanceDynamic* MID;

	FPodiumData Data;

public:
	//~ Begin AActor Override
	virtual void PostInitializeComponents() override; 
	virtual void Tick(float DeltaSeconds) override;
	//~ End AActor Override

protected:
	void OnFinishBlending();

private:
	UPROPERTY(EditAnywhere, Category = "AV Pano")
	float BlendTime;
	
	UPROPERTY(EditAnywhere, Category = "AV Pano")
	float BlendExponent;

	uint8 bStartBlend : 1;
	float BlendAlpha;
	float BlendWeight;

	UPROPERTY(Transient)
	UTexture* Tex;
	UPROPERTY(Transient)
	UTexture* Tex2;
};
