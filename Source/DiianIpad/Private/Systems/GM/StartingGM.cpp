// Powered By Yang


#include "Systems/GM/StartingGM.h"
#include "Player/PC/StartingPC.h"

AStartingGM::AStartingGM(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PlayerControllerClass = AStartingPC::StaticClass();

	PrimaryActorTick.bCanEverTick = false;
}
