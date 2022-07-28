// Powered By Yang


#include "UI/Hack/HGalleryImages.h"
#include "Components/Image.h"

UHGalleryImages::UHGalleryImages(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{ 
	Index = -1;
	CellInfo = nullptr;
}

void UHGalleryImages::Init(int32 CellIndex, FCellInfo* Info)
{ 
	Index = CellIndex;
	CellInfo = Info;
}

void UHGalleryImages::CustomTick(float DeltaSeconds)
{

}

void UHGalleryImages::NativeConstruct()
{ 
	Super::NativeConstruct();

	if (ensure(GalleryImage))
	{
		GalleryImage->SetBrushFromTexture(CellInfo->Tex, true);
	}
}
