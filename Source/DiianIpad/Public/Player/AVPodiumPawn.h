// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "Player/AVPawn.h"
#include "AVPodiumPawn.generated.h"

class AAVPodiumManager;

/**
 * 
 */
UCLASS()
class DIIANIPAD_API AAVPodiumPawn : public AAVPawn
{
	GENERATED_BODY()

public:
	AAVPodiumPawn(const class FObjectInitializer & ObjectInitializer);

protected:
	virtual TSubclassOf<UAVCameraMode> DetermineCameraMode() const override;

	virtual void BeginPlay() override;

	void TryGetPodiumManager();

	void TouchStart(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchEnd(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchMove(ETouchIndex::Type FingerIndex, FVector Location);


	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(Transient)
	AAVPodiumManager* PodiumManager;

	UPROPERTY(Transient)
	class UTourComponent* TourComponent;

};
