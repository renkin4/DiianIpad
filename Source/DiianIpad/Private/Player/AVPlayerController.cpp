// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AVPlayerController.h"
#include "Player/AVPlayerCameraManager.h"
#include "Player/Components/PlayerUIComponent.h"
#include "UObject/ConstructorHelpers.h" 
#include "Blueprint/UserWidget.h"
#include "Components/TouchComponent.h"

AAVPlayerController::AAVPlayerController(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AAVPlayerCameraManager::StaticClass();

	//TouchComponent = CreateDefaultSubobject<UTouchComponent>(TEXT("Touch Component"));
	PlayerUIComp = CreateDefaultSubobject<UPlayerUIComponent>(TEXT("Player UI Comp"));

	static ConstructorHelpers::FClassFinder<UUserWidget> MenuListFinder(TEXT("/Game/UI/Blueprint/Menu/W_MenuList"));
	if (MenuListFinder.Succeeded())
	{
		MenuListClass = MenuListFinder.Class;
	}

	bShowMouseCursor = false;
	bShouldCreateMenuList = true;

	TouchFingerIndex = 0;
	AcceptableDelay = 0.15f;

	PrimaryActorTick.bCanEverTick = true;
}

FVector AAVPlayerController::GetTouchDelta(uint8 FingerIndex)
{
	FVector Delta;

	if (!TouchStartTimes.Contains(FingerIndex) || !TouchCurrentLocations.Contains(FingerIndex) || !TouchStartLocations.Contains(FingerIndex))
	{
		return FVector::ZeroVector;
	}

	const float StartTime = TouchStartTimes[FingerIndex];
	const float EndTime = TouchEndTimes.Contains(FingerIndex) ? TouchEndTimes[FingerIndex] : 0.f;

	if (StartTime <= EndTime + AcceptableDelay) return FVector::ZeroVector;

	const FVector& StartLoc = TouchStartLocations[FingerIndex];
	const FVector& CurrentLoc = TouchCurrentLocations[FingerIndex];

	Delta = CurrentLoc - StartLoc;
	return Delta;
}

FVector AAVPlayerController::GetTouchDeltaNormalized(uint8 FingerIndex)
{

	FVector DeltaNormalized = GetTouchDelta(FingerIndex).GetSafeNormal();
	return DeltaNormalized;
}

FVector AAVPlayerController::GetTouchStartLocation(uint8 FingerIndex)
{

	if (!TouchStartLocations.Contains(FingerIndex))
	{
		return FVector::ZeroVector;
	}

	return TouchStartLocations[FingerIndex];
}

void AAVPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->IsPlayInEditor())
	{
		SetShowMouseCursor(true);
	}

	if(bShouldCreateMenuList)
	{
		MenuList = CreateWidget(this, MenuListClass);
		MenuList->AddToViewport(100);
	}
}

void AAVPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);
	
	InputComponent->BindTouch(IE_Pressed, this, &ThisClass::TouchStart);
	InputComponent->BindTouch(IE_Released, this, &ThisClass::TouchEnd);
	InputComponent->BindTouch(IE_Repeat, this, &ThisClass::TouchMove); 
}

void AAVPlayerController::TouchStart(ETouchIndex::Type FingerIndex, FVector Location)
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	TouchFingerIndex |= 1 << FingerIndex;

	TouchStartTimes.Add(FingerIndex, CurrentTime);
	TouchStartLocations.Add(FingerIndex, Location);
	TouchCurrentLocations.Add(FingerIndex, Location);

	OnTouchStarted.Broadcast(FingerIndex);
}

void AAVPlayerController::TouchEnd(ETouchIndex::Type FingerIndex, FVector Location)
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	if (TapCounter > 0 && FMath::Abs(DoubleTapTimer - CurrentTime) > AcceptableDelay * 2)
	{
		TapCounter = 0;
	}

	TouchFingerIndex &= ~(1 << FingerIndex);

	TouchEndTimes.Add(FingerIndex, CurrentTime);

	const float* StartTime = TouchStartTimes.Find(FingerIndex);
	if (StartTime && *StartTime + AcceptableDelay > CurrentTime)
	{
		OnTouchEnded.Broadcast(FingerIndex);
		if (TapCounter == 0)
		{
			DoubleTapTimer = CurrentTime;
		}

		TapCounter++;

		if (TapCounter >= 2)
		{
			DoubleTap(FingerIndex, Location);
			TapCounter = 0;
		}
	}
	else
	{
		TapCounter = 0;
	}
}

void AAVPlayerController::TouchMove(ETouchIndex::Type FingerIndex, FVector Location)
{
	TouchCurrentLocations.Add(FingerIndex, Location);
}

void AAVPlayerController::DoubleTap(ETouchIndex::Type FingerIndex, FVector Location)
{
	OnDoubleTap.Broadcast(FingerIndex);

}
