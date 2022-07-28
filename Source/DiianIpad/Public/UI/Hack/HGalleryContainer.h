// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "UI/AVUserWidget.h"
#include "Data/AVType.h"
#include "HGalleryContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGalleryFinishLoadingSignature);

class UCanvasPanel;
class UDataTable;
class UHGalleryImages;
class UCanvasPanelSlot;

struct FCellInfo
{
public:
	FCellInfo() = delete;
	FCellInfo(UTexture2D* Tex)
		:Tex(Tex),
		Cell(nullptr)
	{
		// There's some problem with ue4 can't get size right after loading it in editor because it's not cooked. so hacking it to next tick
		/*Size = FVector2D(Tex->GetSizeX(), Tex->GetSizeY());
		AspectRatio = Size.X / Size.Y;*/
	}

	UTexture2D* Tex;
	FVector2D Size;
	float AspectRatio;
	UHGalleryImages* Cell;
	UCanvasPanelSlot* Slot;

public:
	// Hacking it to get size
	bool RefreshSize()
	{
		if (!Tex) return false;

		Size = FVector2D(Tex->GetSizeX(), Tex->GetSizeY());
		AspectRatio = Size.X / Size.Y;
		return true;
	}
};

/**
 * 
 */
UCLASS()
class DIIANIPAD_API UHGalleryContainer : public UAVUserWidget
{
	GENERATED_BODY()
	
public:
	UHGalleryContainer(const class FObjectInitializer & ObjectInitializer);

	UPROPERTY(BlueprintAssignable, Category= "AV Gallery")
	FGalleryFinishLoadingSignature OnFinishLoading;

protected:
	//~ Begin UUserWidget override
	virtual void NativeConstruct() override; 
	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	//~ End UUserWidget override

	void LoadTextures();
	void RefreshSizeNextTick();
	void RecurseRefreshTexture(int32 TexIndex);
	void InitCells();
	
	void ArrangeChild();
	void SetupManager();

	void TryToCenterSelected(float DeltaTime);
	void BlendCellScale(float DeltaTime);
	void OnFinishBlending();

	UFUNCTION()
	void OnSelectedIndexChanges(int32 NewIndex);

	UFUNCTION(BlueprintImplementableEvent , Category="AV")
	void K2_OnSelectedIndexChanges(int32 NewIndex);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "AV Gallery")
	UCanvasPanel * GalleryCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AV Gallery")
	UDataTable * DT_Gallery;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AV Gallery|Cell")
	FVector2D CellPaddings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AV Gallery|Cell")
	TSubclassOf<UHGalleryImages> GalleryCellClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AV Gallery|Cell")
	float MaxYScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AV Gallery|Cell")
	float ScaleModifier; 

	UPROPERTY(BlueprintReadOnly, Category = "AV Gallery|Cell")
	class AAVGalleryManager* Manager;

private:
	TArray<FGalleryCellData*> AllCellData;
	TMap<int32, FCellInfo*> CellMap;

	// Just for ease to get canvas panel slot
	TMap<UHGalleryImages*, UCanvasPanelSlot* > CellSlotMap;

	float ScrollOffset;
	float AccumulatedSize;

	int32 CurrentIndex;
	int32 PreviousIndex;
	
	UPROPERTY(EditAnywhere, Category = "AV Gallery|Cell")
	float BlendTime;
	
	UPROPERTY(EditAnywhere, Category = "AV Gallery|Cell")
	float BlendExponent;

	UPROPERTY(EditAnywhere, Category = "AV Gallery")
	float CenterInterpSpeed;

	uint8 bStartBlend : 1;
	uint8 bInitBlend : 1;
	float BlendAlpha;
	float BlendWeight;

};
