// Powered By Yang

#include "Systems/Managers/AVSurroundingManager.h"
#include "Kismet/GameplayStatics.h"
#include "Surrounding/AVSurroundingBase.h"

AAVSurroundingManager::AAVSurroundingManager(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CheckPitchSubStep = 1 / 30.f;

	PrimaryActorTick.TickInterval = CheckPitchSubStep;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AAVSurroundingManager::RegisterSurroundingComponent(AActor* InActor)
{
	check(InActor);

	AAVSurroundingBase * SurroundingBase = CastChecked<AAVSurroundingBase>(InActor);
	const float TriggerPoint = SurroundingBase->GetTriggerPoint();

	UClass* ActorsClass = InActor->GetClass();
	FPOIInfo* Info = nullptr;

	if (AllRegisteredActors.Contains(ActorsClass))
	{
		Info = AllRegisteredActors[ActorsClass];
	}

	if (!Info)
	{
		Info = new FPOIInfo();
	}

	Info->TriggerPoint = TriggerPoint;
	Info->Class = ActorsClass;

	TSet<AActor*>& ActorSet = Info->RegisteredActor;
	ActorSet.Add(InActor);

	AllRegisteredActors.Add(ActorsClass, Info);
	 
	SurroundingBase->OnForceHidden.Add(FForceHiddenSignature::FDelegate::CreateUObject(this, &ThisClass::OnActorForceHidden));
}

void AAVSurroundingManager::GetActorSetFromClass(UClass* InClass, TSet<AActor*>& OutSet)
{ 
	bool bContainChildClass = false;
	UClass* SelectedClass = nullptr;

	for (const TPair<UClass*, FPOIInfo* > Pair : AllRegisteredActors)
	{
		bContainChildClass = Pair.Key->IsChildOf(InClass);
		SelectedClass = Pair.Key;

		if (bContainChildClass) break;
	}

	if (!bContainChildClass)
	{
		return;
	}

	const FPOIInfo* Info = AllRegisteredActors[SelectedClass];
	OutSet = Info->RegisteredActor;
}

void AAVSurroundingManager::ForceHideActorsFromClass(UClass* InClass, bool bShouldHide /*= false*/)
{ 
	if (FPOIInfo* Info = *AllRegisteredActors.Find(InClass))
	{
		for (AActor* It : Info->RegisteredActor)
		{
			AAVSurroundingBase* SurroundingInterface = CastChecked<AAVSurroundingBase>(It);
			SurroundingInterface->SetForceHidden(bShouldHide);
		}
	}
}

void AAVSurroundingManager::ToggleForceHideActorsFromClass(UClass* InClass)
{
	if (FPOIInfo* Info = *AllRegisteredActors.Find(InClass))
	{
		bool bCurrentHidden = Info->bForceHidden;
		ForceHideActorsFromClass(InClass, !bCurrentHidden);
	}
}

void AAVSurroundingManager::SetClassForceHidden(UClass* InClass, bool bForceHidden /*= false*/)
{ 
	if (FPOIInfo* Info = *AllRegisteredActors.Find(InClass))
	{
		if (Info->bForceHidden == bForceHidden) return;

		Info->bForceHidden = bForceHidden;

		OnForceHiddenChanges.Broadcast(InClass, Info->bForceHidden);
	}
}

void AAVSurroundingManager::BeginPlay()
{
	Super::BeginPlay();

}

void AAVSurroundingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckPlayerCameraPitch();
}

void AAVSurroundingManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{ 
	Super::EndPlay(EndPlayReason);

	for (const TPair<UClass*, FPOIInfo*> Pair : AllRegisteredActors)
	{
		if (Pair.Value)
		{
			delete Pair.Value;
		}
	}

	AllRegisteredActors.Empty();
}

void AAVSurroundingManager::CheckPlayerCameraPitch()
{
	// I don't really care about architecture already fuck this
	const APawn* TargetPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!TargetPawn) return;

	const FRotator& ControllerRot = TargetPawn->GetViewRotation();
	const float CurrentPitch = FMath::ClampAngle(ControllerRot.Pitch, -89.9f, 0.f);	// Fucking hack the numbers in

	for (const TPair<UClass*, FPOIInfo*> Pair : AllRegisteredActors)
	{
		FPOIInfo* Info = Pair.Value;

		if (!Info->bHidden)
		{
			if (Info->TriggerPoint > CurrentPitch) continue;

			Info->HideActor(true);
		}
		else
		{
			if (Info->TriggerPoint <= CurrentPitch) continue;

			Info->HideActor(false);
		}
	}
}

void AAVSurroundingManager::SetHiddenAllRegisterActors(bool bShouldHide)
{
}

void AAVSurroundingManager::OnActorForceHidden(AAVSurroundingBase* SB, bool bShouldHide)
{ 
	ensure(SB);

	UClass* SBClass = SB->GetClass();
	if (FPOIInfo* Info = *AllRegisteredActors.Find(SBClass))
	{
		bool bIsForceHidden = false;

		for (AActor* It : Info->RegisteredActor)
		{
			if (It == SB) continue;

			AAVSurroundingBase* ItSB = CastChecked<AAVSurroundingBase >(It);
			bIsForceHidden |= ItSB->GetForceHidden();

			if (bIsForceHidden) break;
		}

		SetClassForceHidden(SBClass, bIsForceHidden);
	}
}
