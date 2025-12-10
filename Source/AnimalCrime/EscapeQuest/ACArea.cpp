#include "EscapeQuest/ACArea.h"
#include "Game/ACTestGameState.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"

AACArea::AACArea()
{
	PrimaryActorTick.bCanEverTick = false;

	// Root 설정
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);

	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	TriggerBox->SetBoxExtent(FVector(200.f, 200.f, 100.f)); // 기본 크기 설정
	RootComponent = TriggerBox;

	VisualBox = CreateDefaultSubobject<UDecalComponent>(TEXT("VisualBox"));
	VisualBox->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialRef(TEXT("/Game/Project/EscapeQuest/M_Area.M_Area"));
	if (DecalMaterialRef.Succeeded() == true)
	{
		VisualBox->SetDecalMaterial(DecalMaterialRef.Object);
	}

	//기본 숨기기
	SetActorHiddenInGame(true);
}

void AACArea::BeginPlay()
{
	Super::BeginPlay();
	
	//  VisualBox 크기를 TriggerBox 크기에 맞게 설정
	VisualBox->DecalSize = TriggerBox->GetScaledBoxExtent();
	VisualBox->UpdateComponentToWorld();
	VisualBox->MarkRenderStateDirty();

	//auto GS = GetWorld()->GetGameState<ACTestGameState>();
	//if (GS)
	//{
	//	GS->RegisterBombArea(this);
	//}
}

