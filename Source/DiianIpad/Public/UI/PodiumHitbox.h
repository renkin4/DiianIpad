// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "UI/AVUserWidget.h"
#include "PodiumHitbox.generated.h"

/**
 * 
 */
UCLASS()
class DIIANIPAD_API UPodiumHitbox : public UAVUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category="AV")
	AActor * Diu;

	FVector GetDiuLoc() const;

};
