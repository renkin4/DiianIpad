// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Surrounding/AVSurroundingBase.h"
#include "AVRadiusPOI.generated.h"
 
UENUM(BlueprintType)
enum class EPOIType : uint8
{
	NONE = 0 UMETA(Hidden),
	COMMERCIAL,
	EDUCATION,
	MEDICAL,
	PARKS,
};

/**
 * This class is a hack at this point I jsut wanna get the project finished. No planning about dependency or tight coupling
 */
UCLASS()
class DIIANIPAD_API AAVRadiusPOI : public AAVSurroundingBase
{
	GENERATED_BODY()
		
public:
	AAVRadiusPOI(const class FObjectInitializer & ObjectInitializer);

	float GetDistanceAwayFacade() const { return DistanceAwayFacade; }
	
protected:
	//~ Begin 
	virtual void BeginPlay() override;
	virtual void OnFinishedRegistering() override;
	virtual float GetTriggerPoint() const override;
	//~ End 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AV")
	EPOIType POIType;

private:
	float DistanceAwayFacade;

	UPROPERTY(Transient)
	AActor* Facade;

};
