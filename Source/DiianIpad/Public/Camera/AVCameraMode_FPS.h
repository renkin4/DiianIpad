// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "Camera/AVCameraMode.h"
#include "AVCameraMode_FPS.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DIIANIPAD_API UAVCameraMode_FPS : public UAVCameraMode
{
	GENERATED_BODY()

public:
	UAVCameraMode_FPS();

protected:
	//~ UAVCamera Interface
	virtual void UpdateView(float DeltaTime) override;
	//~ End of UAVCamera Interface

};
