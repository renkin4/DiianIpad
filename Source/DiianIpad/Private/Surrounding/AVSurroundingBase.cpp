// Powered By Yang


#include "Surrounding/AVSurroundingBase.h" 
#include "Systems/Managers/AVSurroundingManager.h"
#include "Kismet/GameplayStatics.h"


APawn* AAVSurroundingBase::S_SurroundingCachedPawn = nullptr;

AAVSurroundingBase::AAVSurroundingBase(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	TriggerPoints = -35.f;

	bForceHidden = false;
	bCustomHidden = false;

	PrimaryActorTick.bCanEverTick = false;
}

float AAVSurroundingBase::GetTriggerPoint() const
{
	return TriggerPoints;
}

void AAVSurroundingBase::SetForceHidden(bool bShouldHide)
{ 
	bool bNewHide = bShouldHide;

	bNewHide |= !IsHidden();

	bForceHidden = bShouldHide;

	OnForceHidden.Broadcast(this, bForceHidden);
	K2_ForceHidden.Broadcast(bForceHidden);

	SetActorHiddenInGame(bNewHide);
}

FString AAVSurroundingBase::GetPOIName_Implementation()
{
	return TEXT("Surrounding Base");

}

void AAVSurroundingBase::K2_SetForceHidden(bool bShouldHide /*= true*/)
{
	SetForceHidden(bShouldHide);

}

void AAVSurroundingBase::ToggleForceHidden()
{
	SetForceHidden(!bForceHidden);

}

void AAVSurroundingBase::CustomSetActorHidden(bool bShouldHide /*= false*/)
{
	K2_CustomSetActorHidden(bShouldHide); 
}

void AAVSurroundingBase::PostInitializeComponents()
{
	S_SurroundingCachedPawn = nullptr;

	Super::PostInitializeComponents();
}

void AAVSurroundingBase::BeginPlay()
{
	Super::BeginPlay();
	 
	TryRegisterToSurroundingManager();
	TryGetPawn();
}

void AAVSurroundingBase::SetActorHiddenInGame(bool bNewHidden)
{
	bNewHidden |= bForceHidden;
	CustomSetActorHidden(bNewHidden);

	if (bCustomHidden)
	{
		return;
	}
	Super::SetActorHiddenInGame(bNewHidden);
}

void AAVSurroundingBase::TryRegisterToSurroundingManager()
{
	SurroundingManger = AAVManagerBase::Get<AAVSurroundingManager>();
	static uint8 RetryTimes = 0;

	if (!SurroundingManger)
	{
		check(RetryTimes < 5);

		GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::TryRegisterToSurroundingManager);
		RetryTimes++;
		return;
	}

	SurroundingManger->RegisterSurroundingComponent(this);
	OnFinishedRegistering();
}

void AAVSurroundingBase::TryGetPawn()
{
	S_SurroundingCachedPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if (!S_SurroundingCachedPawn)
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::TryGetPawn);
		return;
	}
}
