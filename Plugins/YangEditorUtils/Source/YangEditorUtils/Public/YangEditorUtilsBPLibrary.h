// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "YangEditorUtilsBPLibrary.generated.h"

class UStaticMeshComponent;

UCLASS()
class UYangEditorUtilsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "YangEditorUtilsTesting", meta = (UnsafeDuringActorConstruction, WorldContext = "WorldContextObject"))
	static void OverrideLightmapResolution(const UObject* WorldContextObject, UStaticMeshComponent * StaticMeshComp, int32 LightmapResolution);

};
