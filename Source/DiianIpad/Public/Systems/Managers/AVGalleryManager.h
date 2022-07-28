// Powered By Yang

#pragma once

#include "CoreMinimal.h" 
#include "AVManagerBase.h"
#include "AVGalleryManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectedIndexSignature, int32, Index);

UCLASS()
class DIIANIPAD_API AAVGalleryManager : public AAVManagerBase
{
	GENERATED_BODY()

public:
	AAVGalleryManager(const class FObjectInitializer & ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category= "AV Gallery")
	void SetSelectedIndex(int32 NewIndex);

	UPROPERTY(BlueprintAssignable, Category= "AV Gallery")
	FSelectedIndexSignature OnSelectedIndexChanged;

private:
	int32 SelectedIndex;

};