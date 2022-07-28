// Copyright Epic Games, Inc. All Rights Reserved.

#include "YangEditorUtilsBPLibrary.h"
#include "YangEditorUtils.h"
#include "Components/StaticMeshComponent.h"

UYangEditorUtilsBPLibrary::UYangEditorUtilsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void UYangEditorUtilsBPLibrary::OverrideLightmapResolution(const UObject* WorldContextObject, UStaticMeshComponent* StaticMeshComp, int32 LightmapResolution)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if(!IsValid(StaticMeshComp) || !World)
	{
		return;
	}

	StaticMeshComp->bOverrideLightMapRes = true;
	StaticMeshComp->OverriddenLightMapRes = FMath::Max(LightmapResolution + 3 & ~3, 4);

	StaticMeshComp->InvalidateLightingCache(); 
}
