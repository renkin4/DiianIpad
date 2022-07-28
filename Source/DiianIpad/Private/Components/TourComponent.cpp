// Powered By Yang
 
#include "Components/TourComponent.h" 
#include "Player/AVPlayerController.h"

UTourComponent::UTourComponent(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	LookUpSpeed = 0.05f;
	TurnSpeed = 0.1f;

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UTourComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{ 
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PCOwner) return;
	RotatePawn(DeltaTime);
}

void UTourComponent::OnRegister()
{  
	Super::OnRegister();
	TryRegisterPC();
}

void UTourComponent::TryRegisterPC()
{
	APawn* Pawn = CastChecked<APawn>(GetOwner());
	if(!Pawn->Controller)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::TryRegisterPC);
		return;
	}

	PCOwner = CastChecked<AAVPlayerController>(Pawn->GetController());
	PCOwner->OnDoubleTap.AddDynamic(this, &ThisClass::OnDoubleTap); 
}

void UTourComponent::RotatePawn(float DeltaTime)
{ 
	// I pretend that Touch Component exist and din't do double checking

	const FVector& DeltaTouch = PCOwner->GetTouchDelta(0);
	const float XDir = DeltaTouch.X;
	const float YDir = DeltaTouch.Y;

	APawn* Pawn = CastChecked<APawn>(GetOwner());

	Pawn->AddControllerPitchInput(YDir * LookUpSpeed * DeltaTime);
	Pawn->AddControllerYawInput(XDir * TurnSpeed * DeltaTime);
}
