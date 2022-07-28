// Powered By Yang

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HGalleryContainer.h"
#include "HGalleryImages.generated.h"

class UTexture2D;
class UImage;

/**
 * 
 */
UCLASS()
class DIIANIPAD_API UHGalleryImages : public UUserWidget
{
	GENERATED_BODY()

public:
	UHGalleryImages(const class FObjectInitializer& ObjectInitializer);

	// dependency ? Who Care about compilation or proper codding practices
	void Init(int32 CellIndex, FCellInfo* Info);

	void CustomTick(float DeltaSeconds);

private:
	int32 Index;
	FCellInfo* CellInfo;

protected:
	//~ Begin
	virtual void NativeConstruct() override;
	//~ End 

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "AV Gallery")
	UImage* GalleryImage;

};
