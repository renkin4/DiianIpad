// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "AVGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DIIANIPAD_API UAVGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	static UAVGameInstance * Get();

	//~ begin Override
	virtual void Init() override;
	virtual void BeginDestroy() override;
	//~ End

public:
	FStreamableManager StreamableManager;

private:
	static UAVGameInstance * s_Instance;
};
