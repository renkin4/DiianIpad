

#include "Surrounding/AVAreas.h"
#include "Components/TextRenderComponent.h"  
#include "GameFramework/PlayerController.h"

AAVAreas::AAVAreas(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	POIName = TEXT("Areas Name");
	Color = FColor::Cyan;

	TextRendererComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Renderer Component"));
	TextRendererComponent->SetRelativeLocation(FVector(0.f, 0.f, 4000.f));
	TextRendererComponent->SetRelativeRotation(FRotator(90.f, 90.f , 0.f) );
	TextRendererComponent->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	TextRendererComponent->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
	TextRendererComponent->WorldSize = 25000.f;

	TextRendererComponent->SetupAttachment(RootComponent);
	
	BlendAlpha = 0.f;
	BlendTime = 1.f;
	BlendExpo = 2.f;
	BlendWeight = 0.f;

	bCustomHidden = true;

	bCHidden = false;
}

void AAVAreas::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotation
	if(APlayerController * PC = GetCachePC())
	{
		const float CameraPitch = PC->PlayerCameraManager->GetCameraRotation().Yaw;
		TextRendererComponent->SetWorldRotation(FRotator(90.f, FRotator::ClampAxis(CameraPitch + 180.f), 0.f));
	}

	DeltaTime *= bCHidden ? -1 : 1;
	BlendAlpha += (DeltaTime / BlendTime);
	BlendAlpha = FMath::Clamp(BlendAlpha, 0.f, 1.f);
	BlendAlpha = FMath::Min(BlendAlpha, 1.0f);
	const float Exponent = (BlendExpo > 0.0f) ? BlendExpo : 1.0f;
	BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);

	MID->SetScalarParameterValue(TEXT("Opacity"), FMath::Lerp(0.f, 0.3f, BlendWeight));
	MID2->SetScalarParameterValue(TEXT("Opacity"), BlendWeight); 
}

void AAVAreas::SetForceHidden(bool bShouldHide)
{
	Super::SetForceHidden(bShouldHide);

	FTimerDelegate TimerDel_SetCHidden;
	TimerDel_SetCHidden.BindUObject(this, &ThisClass::SetCHidden, bShouldHide);

	GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDel_SetCHidden);
}

void AAVAreas::CustomSetActorHidden(bool bShouldHide /*= false*/)
{
	Super::CustomSetActorHidden(bShouldHide);

	bCHidden = bShouldHide;
}

void AAVAreas::SetCHidden(bool bNewHidden /*= false*/)
{
	bCHidden = bNewHidden;
}

void AAVAreas::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	TextRendererComponent->SetText(FText::FromString(POIName.ToUpper()));

	if (!StaticMeshComp->GetStaticMesh() || !StaticMeshComp->GetMaterial(0) || !StaticMeshComp->GetMaterial(1)) return;

	MID = StaticMeshComp->CreateAndSetMaterialInstanceDynamic(0);
	MID2 = StaticMeshComp->CreateAndSetMaterialInstanceDynamic(1);
	 
	MID->SetVectorParameterValue("Color", Color);
	MID2->SetVectorParameterValue("Color", Color);
}

void AAVAreas::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!StaticMeshComp->GetStaticMesh() || !StaticMeshComp->GetMaterial(0) || !StaticMeshComp->GetMaterial(1)) return;

	MID = StaticMeshComp->CreateAndSetMaterialInstanceDynamic(0);
	MID2 = StaticMeshComp->CreateAndSetMaterialInstanceDynamic(1);

}
