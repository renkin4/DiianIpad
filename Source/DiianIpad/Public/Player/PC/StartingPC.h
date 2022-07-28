// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "Player/AVPlayerController.h"
#include "StartingPC.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class DIIANIPAD_API AStartingPC : public AAVPlayerController
{
	GENERATED_BODY()

public:
	AStartingPC(const class FObjectInitializer & ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	
	void InitData();

protected:
	UPROPERTY(BlueprintReadOnly, Category= "AV")
	TArray<UTexture2D*> AllLoadedImages;

private:
	UPROPERTY(EditAnywhere, Category="AV")
	TSubclassOf<UUserWidget> StartingWidgetClass;
	
	UPROPERTY(Transient)
	UUserWidget * StartingWidgetInstance;

	UPROPERTY(EditAnywhere, Category="AV")
	class UDataTable * IS_DataTable;


};
