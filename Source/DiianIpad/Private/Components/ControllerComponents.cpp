// Powered By Yang

#include "Components/ControllerComponents.h"

UControllerComponents::UControllerComponents(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
} 

bool UControllerComponents::IsLocalController() const
{
	return GetControllerChecked<AController>()->IsLocalController();
}

void UControllerComponents::GetPlayerViewPoint(FVector& Location, FRotator& Rotation) const
{
	GetControllerChecked<AController>()->GetPlayerViewPoint(Location, Rotation);
}