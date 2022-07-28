// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "AVManagerBase.generated.h"

UCLASS(notplaceable, BlueprintType, Blueprintable, Transient, hideCategories = (Info, Rendering, MovementReplication, Replication, Actor))
class DIIANIPAD_API AAVManagerBase : public AInfo
{
	GENERATED_BODY()

public:
	AAVManagerBase(const class FObjectInitializer& ObjectInitializer);

	template<typename T>
	static T * Get() { return Cast<T>(s_Instance); }

	UFUNCTION(BlueprintPure, Category = "AV", meta = (DisplayName = "Get Manager Base"))
	static AAVManagerBase* K2_Get() { return Get<AAVManagerBase>(); }

protected:
	//~ Begin Override
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override; 
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End Override

private:
	static AAVManagerBase * s_Instance;

private:
	// Hidden functions that don't make sense to use on this class.
	HIDE_ACTOR_TRANSFORM_FUNCTIONS();

};