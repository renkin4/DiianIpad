// Powered By Yang

#include "Player/AVPawn.h"
#include "Camera/AVCameraComponent.h"
#include "Camera/AVCameraMode.h"
#include "Camera/AVCameraMode_FPS.h"
 
AAVPawn::AAVPawn(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Component"));
	RootComponent = RootSceneComp;
	 
	CameraComponent = CreateDefaultSubobject<UAVCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));

	DefaultCameraMode = UAVCameraMode_FPS::StaticClass();
}

void AAVPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
}

TSubclassOf<UAVCameraMode> AAVPawn::DetermineCameraMode() const
{
	return DefaultCameraMode;
}
