// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/AVUserWidget.h"
#include "UnitDollhouseHitbox.generated.h"

/**
 * 
 */
UCLASS()
class DIIANIPAD_API UUnitDollhouseHitbox : public UAVUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category= "AV")
	int32 Index;

};
