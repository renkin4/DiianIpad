// Powered By Yang


#include "Surrounding/AVRoad.h"
#include "Components/WidgetComponent.h"
#include "UI/RoadWidget.h"

AAVRoad::AAVRoad(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	POIName = TEXT("Road Name");

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Comp"));
	WidgetComp->SetupAttachment(RootComponent);

}

void AAVRoad::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (ensure(WidgetClass) && GetWorld()->IsGameWorld())
	{
		URoadWidget* Widget = CreateWidget<URoadWidget, UWorld>(GetWorld(), WidgetClass);
		Widget->RoadOwner = this;
		WidgetComp->SetWidget(Widget);
	}
}
