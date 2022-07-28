// Powered By Yang


#include "Camera/AVCameraMode_FPS.h"

UAVCameraMode_FPS::UAVCameraMode_FPS()
{

}

void UAVCameraMode_FPS::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.FieldOfView = FieldOfView;
	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
}
