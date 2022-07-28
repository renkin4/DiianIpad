// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "Surrounding/AVSurroundingGeneral.h"
#include "AVRoad.generated.h"

class APlayerController;

/**
 * 
 */
UCLASS()
class DIIANIPAD_API AAVRoad : public AAVSurroundingGeneral
{
	GENERATED_BODY()

public:
	AAVRoad(const class FObjectInitializer& ObjectInitializer);

	//~ Begin Override 
	virtual void PostInitializeComponents() override;
	//~ End Override

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AV")
	class UWidgetComponent* WidgetComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV")
	TSubclassOf<class URoadWidget> WidgetClass;


};
