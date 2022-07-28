// Powered By Yang


#include "Player/AVPodiumPawn.h"
#include "Systems/Managers/AVPodiumManager.h" 
#include "Components/TourComponent.h"

AAVPodiumPawn::AAVPodiumPawn(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	TourComponent = CreateDefaultSubobject<UTourComponent>(TEXT("Tour Component"));

	PrimaryActorTick.bCanEverTick = false;
}

TSubclassOf<UAVCameraMode> AAVPodiumPawn::DetermineCameraMode() const
{
	if(!PodiumManager)
	{
		return DefaultCameraMode;
	}

	return PodiumManager->GetCurrentCameraMode();
}

void AAVPodiumPawn::BeginPlay()
{
	Super::BeginPlay();

	TryGetPodiumManager();
}

void AAVPodiumPawn::TryGetPodiumManager()
{
	PodiumManager = AAVManagerBase::Get<AAVPodiumManager>();

	if(!PodiumManager)
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::TryGetPodiumManager);
		return;
	}
}

void AAVPodiumPawn::TouchStart(ETouchIndex::Type FingerIndex, FVector Location)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Backstabbing bitch"));

}

void AAVPodiumPawn::TouchEnd(ETouchIndex::Type FingerIndex, FVector Location)
{
}

void AAVPodiumPawn::TouchMove(ETouchIndex::Type FingerIndex, FVector Location)
{
}

void AAVPodiumPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	/*Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(InputComponent);

	InputComponent->BindTouch(IE_Pressed, this, &ThisClass::TouchStart);
	InputComponent->BindTouch(IE_Released, this, &ThisClass::TouchEnd);
	InputComponent->BindTouch(IE_Repeat, this, &ThisClass::TouchMove);*/
}
