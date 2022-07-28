// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AVPawn.generated.h"

class UAVCameraComponent;
class UAVCameraMode;

UCLASS()
class DIIANIPAD_API AAVPawn : public APawn
{
	GENERATED_BODY()
	
public:
	AAVPawn(const class FObjectInitializer & ObjectInitializer);
	 
protected:
	virtual void PostInitializeComponents() override;
	
	virtual TSubclassOf<UAVCameraMode> DetermineCameraMode() const;
	
protected:
	// Default camera mode used by player controlled pawns.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	TSubclassOf<UAVCameraMode> DefaultCameraMode;

private: 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AV|Pawn", Meta = (AllowPrivateAccess = "true"))
	UAVCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AV|Pawn", Meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootSceneComp;


};
