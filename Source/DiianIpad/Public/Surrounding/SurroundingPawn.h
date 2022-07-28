// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/AVPawn.h"
#include "SurroundingPawn.generated.h"

/**
 * 
 */
UCLASS()
class DIIANIPAD_API ASurroundingPawn : public AAVPawn
{
	GENERATED_BODY()

public:
	ASurroundingPawn(const class FObjectInitializer & ObjectInitializer);
	
private:
	UPROPERTY(Transient)
	class USurroundingPawnComponent * SurroundingPawnComponent;

};
