// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoadWidget.generated.h"

/**
 * 
 */
UCLASS()
class DIIANIPAD_API URoadWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "AV")
	class AAVSurroundingBase * RoadOwner;

};
