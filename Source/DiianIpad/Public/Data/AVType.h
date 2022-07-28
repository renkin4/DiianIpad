// Powered By Yang

#pragma once

#include "Engine/DataTable.h"
#include "AVType.generated.h"

class UTexture2D;

DECLARE_MULTICAST_DELEGATE_OneParam(FIntDelegate, int32 Val);

USTRUCT(BlueprintType)
struct DIIANIPAD_API FUnitData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FUnitData()
		:Type(TEXT("Type A")),
		Bedroom(4),
		Toilet(3),
		SqFt(500.4f)
	{};

	// Copy Constructor 
	FUnitData(const FUnitData & A)
	{
		Type = A.Type;
		Bedroom = A.Bedroom; 
		Toilet = A.Toilet;
		SqFt = A.SqFt;
		FloorPlan = A.FloorPlan;
		TowerPlan = A.TowerPlan; 
		Specification = A.Specification;
		TypeTex = A.TypeTex;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Unit")
	FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Unit")
	int32 Bedroom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Unit")
	int32 Toilet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Unit")
	float SqFt;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Unit")
	TSoftObjectPtr<UTexture2D> Specification;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Unit")
	TSoftObjectPtr<UTexture2D> FloorPlan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Unit")
	TSoftObjectPtr<UTexture2D> TypeTex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Unit")
	TSoftObjectPtr<UTexture2D> TowerPlan;
	
	friend bool operator==(FUnitData& A, FUnitData& B)
	{
		return (A.Type.Compare(B.Type, ESearchCase::CaseSensitive) == 0)
			&& A.Bedroom  == B.Bedroom
			&& A.Toilet == B.Toilet
			&& A.SqFt == B.SqFt;
	} 

	friend bool operator==(const FUnitData& A,const FUnitData& B)
	{
		return (A.Type.Compare(B.Type, ESearchCase::CaseSensitive) == 0)
			&& A.Bedroom == B.Bedroom
			&& A.Toilet == B.Toilet
			&& A.SqFt == B.SqFt;
	}
};

USTRUCT(BlueprintType)
struct DIIANIPAD_API FImageSequence : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FImageSequence()
		:Index(-1)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV")
	TSoftObjectPtr<UTexture2D> Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV")
	int32 Index;

};

USTRUCT(BlueprintType)
struct DIIANIPAD_API FGalleryCellData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FGalleryCellData()
		:Title(TEXT("Aiyaya")),
		Description(TEXT("Description Aiyaya")),
		Texture(nullptr),
		Index(-1)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Gallery")
	FString Title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Gallery")
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Gallery")
	TSoftObjectPtr<UTexture2D> Texture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Gallery")
	int32 Index;

	friend bool operator==(FGalleryCellData& A, FGalleryCellData& B)
	{
		return A.Texture == B.Texture && A.Index == B.Index;
	}
};


USTRUCT(BlueprintType)
struct DIIANIPAD_API FPodiumData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FPodiumData()
		:Title(TEXT("Podium Title")),
		Description(TEXT("Lorem Lorem Yang Was here")),
		PanoramaTexture(nullptr),
		Location(FVector::ZeroVector)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Podium")
	FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Podium")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Podium")
	TSoftObjectPtr<UTexture2D> PanoramaTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Podium")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Podium")
	TArray<FName> Links;

	friend bool operator==(FPodiumData & A, FPodiumData & B)
	{
		return (A.Title.Compare(B.Title, ESearchCase::CaseSensitive) == 0)
			&& A.PanoramaTexture == B.PanoramaTexture
			&& A.Location == B.Location;
	}
};

USTRUCT(BlueprintType)
struct DIIANIPAD_API FBalconyPanoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FBalconyPanoData()
		:Title(TEXT("Balcony")),
		Index(-1)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Balcony")
	FString Title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Balcony")
	TSoftObjectPtr<UTexture2D> Texture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Balcony")
	int32 Index;
	 
	friend bool operator==(FBalconyPanoData& A, FBalconyPanoData& B)
	{
		return A.Index == B.Index;
	}
};

USTRUCT(BlueprintType)
struct DIIANIPAD_API FUnitPano : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FUnitPano()
		:PanoIndex(-1),
		Type(TEXT("A")),
		WorldPos(FVector::ZeroVector)
	{}
	  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Unit")
	TSoftObjectPtr<UTexture> PanoTex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Unit")
	int32 PanoIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Unit")
	FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Unit")
	FVector WorldPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AV Unit")
	TArray<FName> Links;
};


UENUM(BlueprintType)
enum class EUnitGameplayFeatures : uint8
{
	NONE = 0 UMETA(Hidden),
	TOUR = 1 << 0,
	AERIAL = 1 << 1,
	PERSPECTIVE = 1 << 2,
	BALCONY = 1 << 3,
	EXTRA = 1 << 4,
};