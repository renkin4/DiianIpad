// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "Surrounding/AVSurroundingGeneral.h"
#include "AVAreas.generated.h"

class UMaterialInstanceDynamic;

/**
 * 
 */
UCLASS()
class DIIANIPAD_API AAVAreas : public AAVSurroundingGeneral
{
	GENERATED_BODY()

public:
	AAVAreas(const class FObjectInitializer & ObjectInitializer);

	//~ Begin Override
	virtual void Tick(float DeltaTime) override;
	virtual void SetForceHidden(bool bShouldHide) override; 
	virtual void CustomSetActorHidden(bool bShouldHide = false) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	//~ End Override

	void SetCHidden(bool bNewHidden = false);

private:
	UPROPERTY(VisibleAnywhere, Category="AV")
	class UTextRenderComponent * TextRendererComponent;

	UPROPERTY(EditAnywhere, Category="AV", meta=(DisplayPriority = 1))
	FLinearColor Color;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* MID;
	
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* MID2;

private:
	float BlendAlpha;
	float BlendExpo;
	float BlendTime;
	float BlendWeight;

	uint8 bCHidden : 1;

};
