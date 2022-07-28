// Powered By Yang


#include "Surrounding/AVSurroundingGeneral.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

APlayerController* AAVSurroundingGeneral::s_CachePlayerController = nullptr;

AAVSurroundingGeneral::AAVSurroundingGeneral(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	TriggerPoints = -80.f;
	POIName = TEXT("Surrounding General");

	SceneRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root Comp"));
	RootComponent = SceneRootComp;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	StaticMeshComp->SetupAttachment(RootComponent);

	PrimaryActorTick.TickGroup = ETickingGroup::TG_PostUpdateWork;
	PrimaryActorTick.TickInterval = 1.f / 30.f;
	PrimaryActorTick.bCanEverTick = true;
}

void AAVSurroundingGeneral::BeginPlay()
{
	Super::BeginPlay();

	TryGetPC();
}

void AAVSurroundingGeneral::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (s_CachePlayerController)
	{
		// no need delete ue4 will do it's job
		s_CachePlayerController = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

FString AAVSurroundingGeneral::GetPOIName_Implementation()
{
	return POIName;
}

void AAVSurroundingGeneral::TryGetPC()
{ 
	s_CachePlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (!s_CachePlayerController)
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::TryGetPC);
		return;
	}
}

APlayerController* AAVSurroundingGeneral::GetCachePC()
{
	return s_CachePlayerController;
}

