//// Powered By Yang
//
//
//#include "Components/TouchComponent.h"
//
//// Sets default values for this component's properties
//UTouchComponent::UTouchComponent(const class FObjectInitializer& ObjectInitializer)
//	:Super(ObjectInitializer)
//{  
//	TouchFingerIndex = 0;
//
//	AcceptableDelay = 0.15f;
//
//	PrimaryComponentTick.bCanEverTick = true;
//	PrimaryComponentTick.bStartWithTickEnabled = true;
//}
//
//void UTouchComponent::OnRegister()
//{
//	Super::OnRegister();
//
//	TryInitializePlayerInput();
//}
//
//FVector UTouchComponent::GetTouchDelta(uint8 FingerIndex)
//{
//	FVector Delta;
//
//	if (!TouchStartTimes.Contains(FingerIndex) || !TouchCurrentLocations.Contains(FingerIndex) || !TouchStartLocations.Contains(FingerIndex))
//	{
//		return FVector::ZeroVector;
//	}
//
//	const float StartTime = TouchStartTimes[FingerIndex];
//	const float EndTime = TouchEndTimes.Contains(FingerIndex) ? TouchEndTimes[FingerIndex] : 0.f;
//
//	if (StartTime <= EndTime + AcceptableDelay) return FVector::ZeroVector;
//
//	const FVector& StartLoc = TouchStartLocations[FingerIndex];
//	const FVector& CurrentLoc = TouchCurrentLocations[FingerIndex];
//
//	Delta = CurrentLoc - StartLoc;
//	return Delta;
//}
//
//FVector UTouchComponent::GetTouchDeltaNormalized(uint8 FingerIndex)
//{ 
//	FVector DeltaNormalized = GetTouchDelta(FingerIndex).GetSafeNormal();
//	return DeltaNormalized;
//}
//
//FVector UTouchComponent::GetTouchStartLocation(uint8 FingerIndex)
//{
//
//	if (!TouchStartLocations.Contains(FingerIndex))
//	{
//		return FVector::ZeroVector;
//	}
//
//	return TouchStartLocations[FingerIndex];
//}
//
//void UTouchComponent::TryInitializePlayerInput()
//{ 
//	APlayerController* PlayerC = CastChecked<APlayerController>(GetOwner());
//	if (!PlayerC->InputComponent)
//	{
//		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::TryInitializePlayerInput);
//		return;
//	}
//
//	InitializePlayerInput(PlayerC->InputComponent); 
//}
//
//void UTouchComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
//{
//	check(PlayerInputComponent);
//
//	PlayerInputComponent->BindTouch(IE_Pressed, this, &ThisClass::TouchStart);
//	PlayerInputComponent->BindTouch(IE_Released, this, &ThisClass::TouchEnd);
//	PlayerInputComponent->BindTouch(IE_Repeat, this, &ThisClass::TouchMove);
//}
//
//void UTouchComponent::TouchStart(ETouchIndex::Type FingerIndex, FVector Location)
//{ 
//	const float CurrentTime = GetWorld()->GetTimeSeconds();
//
//	TouchFingerIndex |= 1 << FingerIndex;
//
//	TouchStartTimes.Add(FingerIndex, CurrentTime);
//	TouchStartLocations.Add(FingerIndex, Location);
//	TouchCurrentLocations.Add(FingerIndex, Location);
//
//	OnTouchStarted.Broadcast(FingerIndex);
//}
//
//void UTouchComponent::TouchEnd(ETouchIndex::Type FingerIndex, FVector Location)
//{
//	const float CurrentTime = GetWorld()->GetTimeSeconds();
//
//	if (TapCounter > 0 && FMath::Abs(DoubleTapTimer - CurrentTime) > AcceptableDelay * 2)
//	{
//		TapCounter = 0;
//	}
//
//	TouchFingerIndex &= ~(1 << FingerIndex);
//
//	TouchEndTimes.Add(FingerIndex, CurrentTime);
//
//	const float* StartTime = TouchStartTimes.Find(FingerIndex);
//	if (StartTime && *StartTime + AcceptableDelay > CurrentTime)
//	{
//		OnTouchEnded.Broadcast(FingerIndex);
//		if (TapCounter == 0)
//		{
//			DoubleTapTimer = CurrentTime;
//		}
//
//		TapCounter++;
//
//		if (TapCounter >= 2)
//		{
//			DoubleTap(FingerIndex, Location);
//			TapCounter = 0;
//		}
//	}
//	else
//	{
//		TapCounter = 0;
//	}
//}
//
//void UTouchComponent::TouchMove(ETouchIndex::Type FingerIndex, FVector Location)
//{
//	TouchCurrentLocations.Add(FingerIndex, Location);
//
//}
//
//void UTouchComponent::DoubleTap(ETouchIndex::Type FingerIndex, FVector Location)
//{
//	OnDoubleTap.Broadcast(FingerIndex);
//
//}
