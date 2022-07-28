// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "AVPlayerCameraManager.generated.h"

#define AV_CAMERA_DEFAULT_FOV			(80.0f)
#define AV_CAMERA_DEFAULT_PITCH_MIN		(-89.0f)
#define AV_CAMERA_DEFAULT_PITCH_MAX		(89.0f)

/**
 * 
 */
UCLASS()
class DIIANIPAD_API AAVPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	AAVPlayerCameraManager(const class FObjectInitializer & ObjectInitializer);

};
