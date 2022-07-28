// Fill out your copyright notice in the Description page of Project Settings.


#include "Surrounding/SurroundingPawn.h"
#include "Surrounding/SurroundingPawnComponent.h"


ASurroundingPawn::ASurroundingPawn(const class FObjectInitializer& ObjectInitializer)	
	:Super(ObjectInitializer)
{
	SurroundingPawnComponent = CreateDefaultSubobject<USurroundingPawnComponent>(TEXT("Surrounding Pawn Component"));
	
	PrimaryActorTick.bCanEverTick = false;
}
