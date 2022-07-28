// Powered By Yang


#include "Player/PC/StartingPC.h" 
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "Data/AVType.h"

AStartingPC::AStartingPC(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bShouldCreateMenuList = false;
	PrimaryActorTick.bCanEverTick = true;
}

void AStartingPC::BeginPlay()
{
	Super::BeginPlay();

	InitData();

	if(StartingWidgetClass)
	{
		StartingWidgetInstance = CreateWidget(this, StartingWidgetClass);
		StartingWidgetInstance->AddToPlayerScreen();
	}
}

void AStartingPC::InitData()
{ 
	if (!IS_DataTable)
	{
		FSoftObjectPath DTPath = FSoftObjectPath(TEXT("DataTable'/Game/UI/Assets/StartingScreen/DT_StartingScreenIS'"));
		IS_DataTable = Cast<UDataTable>(DTPath.ResolveObject());

		if (!IS_DataTable)
		{
			IS_DataTable = Cast<UDataTable>(DTPath.TryLoad());
		}

	}

	TArray<FImageSequence*> OutRows;
	IS_DataTable->GetAllRows<FImageSequence>(TEXT(""), OutRows);

	for(const FImageSequence * IS : OutRows)
	{
		UTexture2D * Tex = IS->Image.Get();
		if(!Tex)
		{
			Tex = IS->Image.LoadSynchronous();
		}

		AllLoadedImages.Add(Tex);
	}
}
