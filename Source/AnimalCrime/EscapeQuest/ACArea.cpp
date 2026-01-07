#include "EscapeQuest/ACArea.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

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

	// 나이아가라
	NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
	NiagaraEffect->SetupAttachment(RootComponent);
	NiagaraEffect->SetAutoActivate(true);
	//NiagaraEffect->SetRelativeRotation(FRotator(0.0f, 0.0f, -20.0f));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraRef(TEXT("/Game/Project/Shop/NS_ShopArea.NS_ShopArea"));
	if (NiagaraRef.Succeeded())
	{
		NiagaraEffect->SetAsset(NiagaraRef.Object);
	}

	//기본 숨기기
	SetActorHiddenInGame(true);
}

void AACArea::BeginPlay()
{
	Super::BeginPlay();
}

