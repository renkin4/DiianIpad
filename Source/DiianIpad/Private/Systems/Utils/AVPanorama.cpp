// Powered By Yang


#include "Systems/Utils/AVPanorama.h" 
#include "Components/StaticMeshComponent.h"

static float sPanoScale = 0.5f;
static float sPanoRadius = 30000.f;

// Sets default values
AAVPanorama::AAVPanorama(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PanoRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component Pano"));
	RootComponent = PanoRoot;

	SM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM Comp"));
	SM->SetRelativeRotation(FRotator(0.f, -180.f, 0.f));
	SM->SetRelativeLocation(FVector(0.f, 0.f, sPanoRadius * sPanoScale * -1.f));
	SM->SetRelativeScale3D(FVector(sPanoScale));
	SM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SM->SetGenerateOverlapEvents(false);
	SM->SetupAttachment(RootComponent);

	BlendTime = 1.f;
	BlendWeight = 0.f;
	BlendExponent = 4.f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = ETickingGroup::TG_PostUpdateWork;
}

void AAVPanorama::SetPanoramaText(const TSoftObjectPtr<UTexture>& PanoTexture)
{
	UTexture* LoadedObj = PanoTexture.Get();
	if (!LoadedObj)
	{
		LoadedObj = PanoTexture.LoadSynchronous();
	}

	if (Tex == nullptr)
	{
		Tex = LoadedObj;
		MID->SetTextureParameterValue(TEXT("Tex"), Tex);
		return;
	}

	Tex2 = LoadedObj;

	MID->SetTextureParameterValue(TEXT("Tex2"), Tex2);
	MID->SetScalarParameterValue(TEXT("Alpha"), BlendWeight);

	bStartBlend = true;
	BlendAlpha = 0.f;
}


void AAVPanorama::SetPanoData(const FPodiumData& PanoData)
{
	if (&Data == &PanoData) return;

	Data = PanoData;
	//SetActorLocation(Data.Location);

	SetPanoramaText(Data.PanoramaTexture);
}

void AAVPanorama::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!SM->GetStaticMesh()) return;

	MID = UMaterialInstanceDynamic::Create(SM->GetMaterial(0), this);
	SM->SetMaterial(0, MID);
}

void AAVPanorama::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bStartBlend)
	{
		BlendAlpha += (DeltaSeconds / BlendTime);
		BlendAlpha = FMath::Min(BlendAlpha, 1.0f);
		const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);

		//GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::White, FString::Printf(TEXT("Blend Weight : %f"), BlendWeight));

		MID->SetScalarParameterValue(TEXT("Alpha"), BlendWeight);

		if (BlendWeight >= 1.f)
		{
			OnFinishBlending();
		}
	}
}

void AAVPanorama::OnFinishBlending()
{
	bStartBlend = false;
	BlendAlpha = 0.f;
	BlendWeight = 0.f;

	Tex = Tex2;
	MID->SetTextureParameterValue(TEXT("Tex"), Tex);
	MID->SetScalarParameterValue(TEXT("Alpha"), BlendWeight);
}
