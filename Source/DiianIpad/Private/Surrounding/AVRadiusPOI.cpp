// Fill out your copyright notice in the Description page of Project Settings.


#include "Surrounding/AVRadiusPOI.h"
#include "Kismet/GameplayStatics.h"

AAVRadiusPOI::AAVRadiusPOI(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAVRadiusPOI::BeginPlay()
{
	Super::BeginPlay();
}

void AAVRadiusPOI::OnFinishedRegistering()
{
	TArray<AActor*> FacadeArray;
	UGameplayStatics::GetAllActorsWithTag(this, TEXT("Facade"), FacadeArray);

	for (AActor* It : FacadeArray)
	{
		Facade = It;
	}

	if (ensure(Facade))
	{
		DistanceAwayFacade = (Facade->GetActorLocation() - GetActorLocation()).Size();
	}
}

float AAVRadiusPOI::GetTriggerPoint() const
{
	return 0.f;
}
