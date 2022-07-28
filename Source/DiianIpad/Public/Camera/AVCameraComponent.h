// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameplayTagContainer.h"
#include "AVCameraComponent.generated.h"

class UAVCameraMode;
class UAVCameraModeStack; 

DECLARE_DELEGATE_RetVal(TSubclassOf<UAVCameraMode>, FAVCameraModeDelegate);

/**
 * 
 */
UCLASS()
class DIIANIPAD_API UAVCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
public:
	UAVCameraComponent(const FObjectInitializer& ObjectInitializer);

	// Returns the camera component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "Archviz Core|Camera")
	static UAVCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UAVCameraComponent>() : nullptr); }
	 
	// Returns the target actor that the camera is looking at.
	virtual AActor* GetTargetActor() const { return GetOwner(); }

	// Delegate used to query for the best camera mode.
	FAVCameraModeDelegate DetermineCameraModeDelegate;
	 
	// Add an offset to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
	void AddFieldOfViewOffset(float FovOffset) { FieldOfViewOffset += FovOffset; }

	virtual void DrawDebug(UCanvas* Canvas) const;

	// Gets the tag associated with the top layer and the blend weight of it
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;

protected:

	virtual void OnRegister() override;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	virtual void UpdateCameraModes();

protected: 
	// Stack used to blend the camera modes.
	UPROPERTY()
	UAVCameraModeStack* CameraModeStack;

	// Offset applied to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
	float FieldOfViewOffset;

};
