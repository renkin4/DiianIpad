// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "AVGameMode.generated.h"

class AAVManagerBase;

/**
 * 
 */
UCLASS()
class DIIANIPAD_API AAVGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AAVGameMode(const class FObjectInitializer & ObjectInitializer);
	


protected:
	//~ Begin Override
	virtual void PreInitializeComponents() override;
	//~ End Override

private:
	UPROPERTY(EditAnywhere, Category = "AV")
	TArray<TSubclassOf<AAVManagerBase>> ManagersToSpawn;

};
