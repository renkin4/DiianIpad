// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "Camera/AVCameraMode.h"
#include "Curves/CurveFloat.h"
#include "AVCameraMode_Aerial.generated.h"

class UCurveVector;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class DIIANIPAD_API UAVCameraMode_Aerial : public UAVCameraMode
{
	GENERATED_BODY()
	
public: 
	UAVCameraMode_Aerial();

protected:
	virtual void UpdateView(float DeltaTime) override;


protected: 
	// Curve that defines local-space offsets from the target using the view pitch to evaluate the curve.
	UPROPERTY(EditDefaultsOnly, Category = "Target Offset", Meta = (EditCondition = "!bUseRuntimeFloatCurves"))
	const UCurveVector* TargetOffsetCurve;

	// UE-103986: Live editing of RuntimeFloatCurves during PIE does not work (unlike curve assets).
	// Once that is resolved this will become the default and TargetOffsetCurve will be removed.
	UPROPERTY(EditDefaultsOnly, Category = "Target Offset")
	uint8 bUseRuntimeFloatCurves : 1;

	UPROPERTY(EditDefaultsOnly, Category = "Target Offset", Meta = (EditCondition = "bUseRuntimeFloatCurves"))
	FRuntimeFloatCurve TargetOffsetX;

	UPROPERTY(EditDefaultsOnly, Category = "Target Offset", Meta = (EditCondition = "bUseRuntimeFloatCurves"))
	FRuntimeFloatCurve TargetOffsetY;

	UPROPERTY(EditDefaultsOnly, Category = "Target Offset", Meta = (EditCondition = "bUseRuntimeFloatCurves"))
	FRuntimeFloatCurve TargetOffsetZ;

	//virtual FVector GetPivotLocation() const override;

};
