// Powered By Yang


#include "Surrounding/RingTools.h"
#include "Components/TextRenderComponent.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Systems/Managers/AVSurroundingManager.h"
#include "Kismet/GameplayStatics.h"
#include "Surrounding/AVRadiusPOI.h"

#define LOCTEXT_NAMESPACE "Ring Tool"

#define OneKM 100000.0f

static APawn* S_Pawn = nullptr;

FName ARingTools::ProceduralMeshName(TEXT("Procedural Mesh Name"));
FName ARingTools::TextRendererName(TEXT("Text Renderer Name"));

ARingTools::ARingTools(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{ 
	ProceduralMeshComp = ObjectInitializer.CreateDefaultSubobject<UProceduralMeshComponent>(this, ProceduralMeshName);
	RootComponent = ProceduralMeshComp;

	TextRendererComp = ObjectInitializer.CreateDefaultSubobject<UTextRenderComponent>(this, TextRendererName);
	TextRendererComp->SetRelativeLocation(FVector(0, 0, 25500.f));
	TextRendererComp->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	TextRendererComp->SetHorizontalAlignment(EHTA_Center);
	TextRendererComp->SetWorldSize(35000.f);
	TextRendererComp->SetupAttachment(RootComponent);

	/************************************************************************/
	/* Default value setup                                                  */
	/************************************************************************/
	NoOfSegments = 64;
	Radius = 100000.0f;
	RingColor = FColor::Cyan;
	LagMaxTimeStep = 1.f / 60.f;
	RotationLagSpeed = 5.f;

	TriggerPoints = -45.f;

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

FString ARingTools::GetPOIName_Implementation()
{
	return FString::Printf(TEXT("%d KM"), (int32)(Radius * 0.00001f));
}

void ARingTools::PostInitializeComponents()
{ 
	S_Pawn = nullptr;
	Super::PostInitializeComponents();
}

void ARingTools::BeginPlay()
{
	Super::BeginPlay();

	TryGetPC();
}

void ARingTools::Tick(float DeltaSec)
{
	Super::Tick(DeltaSec);

	if (IsValid(SurroundingPC))
	{
		FVector CameraLoc = SurroundingPC->PlayerCameraManager->GetCameraLocation();
		FVector NewDir = CameraLoc - GetActorLocation();

		static FRotator s_NewRot;
		s_NewRot = FRotationMatrix::MakeFromX(NewDir).Rotator();
		s_NewRot.Yaw -= 35.f;

		TargetRotation = FRotator(0.f, s_NewRot.Yaw, 0.f);
	}

	UpdateTargetRotation(DeltaSec);
}

void ARingTools::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	static float theta, x1, y1, x2, y2;
	theta = PI * 2 / NoOfSegments;

	TArray<FVector> vertices;
	TArray<FVector2D> uv;
	TArray<int32> triangles;

	for (int32 i = 0; i < NoOfSegments; ++i)
	{
		x1 = FMath::Sin(i * theta) * Radius;
		y1 = FMath::Cos(i * theta) * Radius;

		x2 = FMath::Sin((i + 1) * theta) * Radius;
		y2 = FMath::Cos((i + 1) * theta) * Radius;

		vertices.Add(FVector::ZeroVector);
		vertices.Add(FVector(x1, y1, 0.f));
		vertices.Add(FVector(x2, y2, 0.f));

		uv.Add(FVector2D::ZeroVector);
		uv.Add(FVector2D(1.f, 0.f));
		uv.Add(FVector2D(1.f, 1.f));
	}

	// Probably can stick them together for 1 iteration 
	for (int32 i = 0; i < NoOfSegments * 3; ++i)
	{
		static int32 verticesIndex;
		verticesIndex = i % 3 == 0 ? 0 : i;

		triangles.Add(verticesIndex);
	}

	TArray<FVector> emptyVector;
	TArray<FColor> emptyVertexColor;
	TArray<FProcMeshTangent> emptyProcMeshTangent;

	ProceduralMeshComp->CreateMeshSection(0, vertices, triangles, emptyVector, uv, emptyVertexColor, emptyProcMeshTangent, false);

	if (MaterialInterface)
	{
		MID = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, MaterialInterface);
	}

	if (MID)
	{
		MID->SetVectorParameterValue(TEXT("RingColor"), RingColor);
		ProceduralMeshComp->SetMaterial(0, MID);
	}

	// TODO Set TExt in KM
	FString kmInString = FString::Printf(TEXT("%d KM"), (int32)(Radius * 0.00001f));

	TextRendererComp->SetText(FText::FromString(kmInString));
	TextRendererComp->SetRelativeLocation(FVector(Radius * -1, 0.f, 0.f));
}

void ARingTools::SetActorHiddenInGame(bool bNewHidden)
{
	bool PreviousHiddenStatus = IsHidden();
	Super::SetActorHiddenInGame(bNewHidden);

	bNewHidden |= bForceHidden;

	if (PreviousHiddenStatus == bNewHidden)
	{
		return;
	}

	HackPOI(bNewHidden);
}

void ARingTools::OnFinishedRegistering()
{
	GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::GetAllPOIRadiusNextTick);

	FPOIInfo* Info = SurroundingManger->GetPOIInfo(GetClass());
	Info->RegisteredActor.Sort([](AActor& A, AActor& B) {
		ARingTools* RTA = CastChecked<ARingTools>(&A);
		ARingTools* RTB = CastChecked<ARingTools>(&B);

		return RTA->GetRadius() < RTB->GetRadius();
	});
}

void ARingTools::PivotToFacade()
{

}

void ARingTools::UpdateTargetRotation(float DeltaSec)
{

	FRotator DesiredRot = TargetRotation;

	const FRotator ArmRotStep = (DesiredRot - PreviousTargetRot).GetNormalized() * (1.f / DeltaSec);
	FRotator LerpTarget = PreviousTargetRot;
	float RemainingTime = DeltaSec;
	while (RemainingTime > KINDA_SMALL_NUMBER)
	{
		const float LerpAmount = FMath::Min(LagMaxTimeStep, RemainingTime);
		LerpTarget += ArmRotStep * LerpAmount;
		RemainingTime -= LerpAmount;

		DesiredRot = FRotator(FMath::QInterpTo(FQuat(PreviousTargetRot), FQuat(LerpTarget), LerpAmount, RotationLagSpeed));
		PreviousTargetRot = DesiredRot;

		SetActorRotation(DesiredRot);
	}
}

void ARingTools::GetAllPOIRadiusNextTick()
{
	SurroundingManger->GetActorSetFromClass(AAVRadiusPOI::StaticClass(), AllPOIRadius);
	if (AllPOIRadius.Num() <= 0)
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::GetAllPOIRadiusNextTick);
	}
}

void ARingTools::TryGetPC()
{
	SurroundingPC = UGameplayStatics::GetPlayerController(this, 0);

	if (!SurroundingPC)
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::TryGetPC);
		return;
	}
}

void ARingTools::HackPOI(bool bNewHidden)
{
	if(AllPOIRadius.Num() <= 0)
	{
		FTimerDelegate TimerDel_HackPOI;
		TimerDel_HackPOI.BindUObject(this, &ThisClass::HackPOI, bNewHidden);

		FTimerHandle TimerHandler;

		GetWorldTimerManager().SetTimer(TimerHandler, TimerDel_HackPOI, 0.2f, false);
		return;
	}

	// Hack
	for (AActor* POI : AllPOIRadius)
	{
		AAVRadiusPOI* POIRadius = CastChecked<AAVRadiusPOI>(POI);
		const float DistanceAway = POIRadius->GetDistanceAwayFacade();
		const float A = DistanceAway - Radius;

		bool bInRadius = (A < 0.f && A >= -OneKM);
		if (!bInRadius) continue;

		POI->SetActorHiddenInGame(bNewHidden);
	}
}

#undef LOCTEXT_NAMESPACE