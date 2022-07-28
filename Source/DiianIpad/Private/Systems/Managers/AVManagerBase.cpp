// Powered By Yang

#include "Systems/Managers/AVManagerBase.h"

AAVManagerBase* AAVManagerBase::s_Instance = nullptr;

AAVManagerBase::AAVManagerBase(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

}

void AAVManagerBase::PostInitializeComponents()
{
	ensure(!s_Instance);
	s_Instance = this;
	
	Super::PostInitializeComponents();
}

void AAVManagerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAVManagerBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	s_Instance = nullptr;

	Super::EndPlay(EndPlayReason);
}
