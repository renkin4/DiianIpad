// Fill out your copyright notice in the Description page of Project Settings.


#include "Surrounding/SurroundingPawnComponent.h"
#include "Player/AVPlayerController.h"

#define PITCH_SPEED 1.f

USurroundingPawnComponent::USurroundingPawnComponent(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PanBoxExtend = FVector(500000, 500000, 2000);

	bStartPitchingUp = false;
	AmountToPitch = 0.f;

	PrimaryComponentTick.bCanEverTick = true; 
}

void USurroundingPawnComponent::OnRegister()
{
	Super::OnRegister();

	const APawn* Pawn = CastChecked<APawn>(GetOwner());
	OwnerInitialLocation = Pawn->GetActorLocation();
	TargetPanLocation = OwnerInitialLocation;

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::RotatePawnNextTick);
}

void USurroundingPawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PCOwner) return; 

	PanPawn(DeltaTime);
	CheckPawnPosition(DeltaTime);
	RotatePawnToNorth(DeltaTime); 


	if (bStartPitchingUp)
	{
		APawn* Pawn = CastChecked<APawn>(GetOwner());
		const float ConsumeAmount = ConstAmountToPitch * (DeltaTime * PITCH_SPEED);

		Pawn->Controller->SetControlRotation(FRotator(Pawn->Controller->GetControlRotation().Pitch + ConsumeAmount, -90.f, 0.f) );
		AmountToPitch = AmountToPitch - ConsumeAmount;


		if (AmountToPitch >= 0.f)
		{
			bStartPitchingUp = false;
		}
	}

}

void USurroundingPawnComponent::RotatePawn(float DeltaTime)
{ 
	if (bIsPanning || bStartPitchingUp) return;

	const FVector& DeltaTouch = PCOwner->GetTouchDelta(0);
	const float XDir = DeltaTouch.X;
	const float YDir = DeltaTouch.Y;

	APawn* Pawn = CastChecked<APawn>(GetOwner());

	Pawn->AddControllerPitchInput(YDir * LookUpSpeed * DeltaTime);

	const FRotator& PawnRot = Pawn->GetControlRotation();
	if (FRotator::NormalizeAxis(PawnRot.Pitch) < -80.f)
	{
		return;
	}

	Pawn->AddControllerYawInput(XDir * TurnSpeed * DeltaTime);
}

void USurroundingPawnComponent::PanPawn(float DeltaTime)
{
	if (!PCOwner || !CanPan()) return;

	APawn* Pawn = CastChecked<APawn>(GetOwner());

	FVector NewLoc = FMath::VInterpTo(Pawn->GetActorLocation(), TargetPanLocation, DeltaTime, 2.f);
	Pawn->SetActorLocation(NewLoc);
}

bool USurroundingPawnComponent::CanPan() const
{
	const APawn* Pawn = CastChecked<APawn>(GetOwner());
	const FRotator& PawnRot = Pawn->GetControlRotation();

	// Hard code who gonna know
	const float CurrentPitch = FRotator::NormalizeAxis(PawnRot.Pitch);

	return CurrentPitch < -80.f;
}

void USurroundingPawnComponent::CheckPawnPosition(float DeltaTime)
{
	// Checking every tick who cares 
	APawn* Pawn = CastChecked<APawn>(GetOwner());
	const FRotator& PawnRot = Pawn->GetControlRotation();
	const float CurrentPitch = FRotator::NormalizeAxis(PawnRot.Pitch);

	if (CurrentPitch < -60.f) return;

	TargetPanLocation = OwnerInitialLocation;
	const FVector ConsumeAmount = (OwnerInitialLocation - Pawn->GetActorLocation()) * DeltaTime;

	Pawn->SetActorLocation(ConsumeAmount + Pawn->GetActorLocation());
}

void USurroundingPawnComponent::RotatePawnToNorth(float DeltaTime)
{
	APawn* Pawn = CastChecked<APawn>(GetOwner());
	FRotator PawnRot = Pawn->GetControlRotation();
	if (FRotator::NormalizeAxis(PawnRot.Pitch) >= -80.f)
	{
		return;
	}

	FRotator NewRot = FMath::RInterpTo(PawnRot, FRotator(PawnRot.Pitch, -90.f, 0.f), DeltaTime, 2.f);
	Pawn->Controller->SetControlRotation(NewRot);
}

void USurroundingPawnComponent::OnDoubleTap(uint8 FingerIndex)
{
	APawn* Pawn = CastChecked<APawn>(GetOwner());
	AAVPlayerController* PC = CastChecked<AAVPlayerController>(Pawn->GetController());

	FVector TouchLoc = PC->GetTouchStartLocation(FingerIndex);

	FVector WorldLoc, WorldDir;
	bool bSucceeded = PC->DeprojectScreenPositionToWorld(TouchLoc.X, TouchLoc.Y, WorldLoc, WorldDir);
	if (!bSucceeded) return;

	FHitResult OutHit;
	GetWorld()->LineTraceSingleByChannel(OutHit, WorldLoc, WorldLoc + (WorldDir * 999999999999.f), ECollisionChannel::ECC_Visibility);

	if (!OutHit.bBlockingHit) return;

	TargetPanLocation = PanAABB(FVector(OutHit.Location.X, OutHit.Location.Y, Pawn->GetActorLocation().Z));
}

void USurroundingPawnComponent::StartPitching()
{ 
	APawn* Pawn = CastChecked<APawn>(GetOwner());
	const float CurrentPitch = FRotator::NormalizeAxis(Pawn->Controller->GetControlRotation().Pitch);
	const float TargetPitch = -90.f;

	AmountToPitch = TargetPitch - CurrentPitch;
	ConstAmountToPitch = AmountToPitch;

	bStartPitchingUp = true;
}

void USurroundingPawnComponent::RotatePawnNextTick()
{ 
	APawn* Pawn = CastChecked<APawn>(GetOwner());

	static int32 s_RetryTimes = 0;

	if(!Pawn || !Pawn->Controller) 
	{
		if (s_RetryTimes > 5)
		{
			return;
		}

		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::RotatePawnNextTick);
		s_RetryTimes ++;
		return;
	}

	FRotator NewRot = FRotator(-35.f, Pawn->Controller->GetControlRotation().Yaw, 0.f);
	Pawn->Controller->SetControlRotation(NewRot);

	FTimerHandle TimerHandler;
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ThisClass::StartPitching, 1.f, false);
}

FVector USurroundingPawnComponent::PanAABB(const FVector& NewLoc)
{
	const FVector A = OwnerInitialLocation - PanBoxExtend;
	const FVector B = OwnerInitialLocation + PanBoxExtend;

	float X, Y, Z;
	X = FMath::Clamp(NewLoc.X, A.X, B.X);
	Y = FMath::Clamp(NewLoc.Y, A.Y, B.Y);
	Z = FMath::Clamp(NewLoc.Z, A.Z, B.Z);

	const FVector OutVector = FVector(X, Y, Z);
	return OutVector;

}
