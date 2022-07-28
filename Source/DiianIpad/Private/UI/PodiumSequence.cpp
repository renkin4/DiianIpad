// Powered By Yang


#include "UI/PodiumSequence.h"
#include "Systems/Managers/AVPodiumManager.h"
#include "DatasmithAssetUserData.h"
#include "UI/PodiumHitbox.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Kismet/KismetSystemLibrary.h"

UPodiumSequence::UPodiumSequence(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UPodiumSequence::Init()
{
	AllDius.Empty();

	PodiumManager->GetAllDiu(AllDius);

	if(!HitBoxClass) return;
	check(SequenceCanvas);

	AllPodiumHitboxes.Empty();

	for (AActor* It : AllDius)
	{
		UDatasmithAssetUserData* DataSmithData = UDatasmithAssetUserData::GetDatasmithUserData(It);
		bool bContainPodiumID = DataSmithData->MetaData.Contains(TEXT("PodiumID"));

		if (!bContainPodiumID)
		{
			continue;
		}

		UPodiumHitbox* PodiumHB = CreateWidget<UPodiumHitbox>(GetOwningPlayer(), HitBoxClass);
		PodiumHB->Diu = It;

		UCanvasPanelSlot* CSlot = SequenceCanvas->AddChildToCanvas(PodiumHB);
		CSlot->SetAlignment(FVector2D(.5f, .5f));
		CSlot->SetSize(FVector2D(32.f, 32.f));
		CSlot->SetZOrder(10);

		AllPodiumHitboxes.Add(PodiumHB);
	}

}

void UPodiumSequence::NativePreConstruct()
{
	Super::NativePreConstruct();

}

void UPodiumSequence::NativeConstruct()
{
	Super::NativeConstruct();

	TryGetPodiumManager();
}

void UPodiumSequence::TryGetPodiumManager()
{
	PodiumManager = AAVManagerBase::Get<AAVPodiumManager>();

	if(!PodiumManager)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::TryGetPodiumManager);
		return;
	}

	Init();

	PodiumManager->OnPodiumIndexChages.AddDynamic(this, &ThisClass::OnPodiumIndexChanges);
	PodiumManager->OnPodiumStateChanges.AddDynamic(this, &ThisClass::OnPodiumChangeStates);

	if(PodiumManager->GetFinishLoadingTextures())
	{
		OnFinishLoadingTextures();
	}
	else
	{	
		PodiumManager->OnFinishLoadingTextures.AddDynamic(this, &ThisClass::OnFinishLoadingTextures);
	}
}

void UPodiumSequence::ProjectHitBoxes()
{
	const FVector & OwnerCamLoc = GetOwningPlayer()->PlayerCameraManager->GetCameraLocation();

	for(UPodiumHitbox* HB : AllPodiumHitboxes)
	{
		const FVector & DiuLoc = HB->GetDiuLoc();

		FHitResult OutHit;
		GetWorld()->LineTraceSingleByChannel(OutHit, OwnerCamLoc, DiuLoc, ECC_Visibility, FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam);

		FVector2D ScreenLoc;
		GetOwningPlayer()->ProjectWorldLocationToScreen(DiuLoc, ScreenLoc, false);

		FVector2D NewLoc = GetPaintSpaceGeometry().AbsoluteToLocal(ScreenLoc);

		UCanvasPanelSlot * CSlot = CastChecked<UCanvasPanelSlot>(HB->Slot);
		CSlot->SetPosition(NewLoc);

		ESlateVisibility NewVisibility = OutHit.bBlockingHit ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible;
		HB->SetVisibility(NewVisibility);
	}
}

void UPodiumSequence::OnFinishLoadingTextures()
{
	PodiumManager->ChangeIndex(0);

	FTimerHandle TimerHandler; 
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ThisClass::ProjectHitBoxes, 0.2f);
}

void UPodiumSequence::OnPodiumIndexChanges(int32 NewIndex, int32 OldIndex)
{
	check(SequenceImage);

	UTexture2D * Tex = PodiumManager->LoadedImages[NewIndex];
	SequenceImage->SetBrushFromTexture(Tex, true);
	ProjectHitBoxes();
}

void UPodiumSequence::OnPodiumChangeStates(EPodiumState NewState, EPodiumState OldState)
{
	switch (NewState)
	{
	case EPodiumState::NONE:
		break;
	case EPodiumState::SEQUENCER:
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		break;
	case EPodiumState::VR:
		SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EPodiumState::GROUND:
		SetVisibility(ESlateVisibility::Collapsed);
		break;
	default:
		break;
	}
}

void UPodiumSequence::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	float TouchDeltaNormalized = GetDeltaTouchNormalized().X;
	float TouchDelta = GetDeltaTouch().X;
	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Delta : %f, Nromalized : %f"), TouchDelta, TouchDeltaNormalized));
	if (TouchDeltaNormalized != 0.f && FMath::Abs(TouchDelta) > 50.f)
	{
		int32 Sign = FMath::Sign(TouchDeltaNormalized) * -1;
		PodiumManager->UpdateCurrentIndex(Sign);
	}
}

FReply UPodiumSequence::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply SuperReply = Super::NativeOnTouchMoved(InGeometry, InGestureEvent);


	return SuperReply;
}
