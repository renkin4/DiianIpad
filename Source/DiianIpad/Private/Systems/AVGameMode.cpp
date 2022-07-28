// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/AVGameMode.h"
#include "Player/AVPlayerController.h"
#include "Player/AVPawn.h"
#include "UI/AVHUD.h"
#include "Systems/Managers/AVManagerBase.h"

AAVGameMode::AAVGameMode(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	DefaultPawnClass = AAVPawn::StaticClass();
	PlayerControllerClass = AAVPlayerController::StaticClass();
	HUDClass = AAVHUD::StaticClass();

	PrimaryActorTick.bCanEverTick = false;
}

void AAVGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	 
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save game states or network managers into a map									
	UWorld* World = GetWorld();

	for(const TSubclassOf<AAVManagerBase> & ManagerClass : ManagersToSpawn)
	{ 
		World->SpawnActor<AAVManagerBase>(ManagerClass, SpawnInfo); // Just spawn they will handle themselve
	}

}
