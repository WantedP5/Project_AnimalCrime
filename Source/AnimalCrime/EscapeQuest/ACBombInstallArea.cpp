#include "ACBombInstallArea.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "Item/ACEscapeMissionBomb.h"

#include "AnimalCrime.h"

ACBombInstallArea::ACBombInstallArea()
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

}

void ACBombInstallArea::BeginPlay()
{
	Super::BeginPlay();

	//  VisualBox 크기를 TriggerBox 크기에 맞게 설정
	VisualBox->DecalSize = TriggerBox->GetScaledBoxExtent();
	VisualBox->UpdateComponentToWorld();
	VisualBox->MarkRenderStateDirty();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACBombInstallArea::OnBombTriggerOverlapBegin);
}

void ACBombInstallArea::OnBombTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AACEscapeMissionBomb* OverlappedBomb = Cast<AACEscapeMissionBomb>(OtherActor);
	if (OverlappedBomb == nullptr)
	{
		AC_LOG(LogSY, Log, TEXT("Check"));
		return;
	}

	//루트 컴포넌트가 아닌 경우 리턴
	if (OtherComp != OverlappedBomb->GetRootComponent())
	{
		AC_LOG(LogSY, Log, TEXT("Cut"));
		return;
	}

	AC_LOG(LogSY, Log, TEXT("Bomb entered install area"));
}
