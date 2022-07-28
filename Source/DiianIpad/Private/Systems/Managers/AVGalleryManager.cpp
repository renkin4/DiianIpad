// Powered By Yang

#include "Systems/Managers/AVGalleryManager.h"

AAVGalleryManager::AAVGalleryManager(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SelectedIndex = -1;
	PrimaryActorTick.bCanEverTick = false;
}

void AAVGalleryManager::SetSelectedIndex(int32 NewIndex)
{
	if (NewIndex == SelectedIndex)
	{
		return;
	}

	SelectedIndex = NewIndex;
	OnSelectedIndexChanged.Broadcast(SelectedIndex); 
}
