// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TourComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIIANIPAD_API UTourComponent : public UActorComponent
{
	GENERATED_BODY()

public:	 
	UTourComponent(const class FObjectInitializer& ObjectInitializer);

	//~ UActorComponent interface
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~ End of UActorComponent interface
	
protected:
	//~ UActorComponent Interface
	virtual void OnRegister() override;
	//~ End of UActorComponent Interface

	void TryRegisterPC();

	virtual void RotatePawn(float DeltaTime); 

	UFUNCTION()
	virtual void OnDoubleTap(uint8 FingerIndex) {}

protected:
	UPROPERTY(EditAnywhere, Category="Aiyaya")
	float LookUpSpeed;
	UPROPERTY(EditAnywhere, Category="Aiyaya")
	float TurnSpeed;

protected:
	UPROPERTY(Transient)
	class AAVPlayerController* PCOwner;
};
