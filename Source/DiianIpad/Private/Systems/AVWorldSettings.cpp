// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/AVWorldSettings.h"

AAVWorldSettings::AAVWorldSettings(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{ 
	PrimaryActorTick.bCanEverTick = false;
}
