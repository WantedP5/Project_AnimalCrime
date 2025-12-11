
#include "ACTestBlackMarketDealer.h"
#include "ACTestMafiaCharacter.h"
#include "Components/BoxComponent.h"
#include "EscapeQuest/ACBlackMarketComponent.h"
#include "Component/ACInteractableComponent.h"
#include "AnimalCrime.h"

AACTestBlackMarketDealer::AACTestBlackMarketDealer()
{
	HeadMesh->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Hairstyle_male_010.SK_Hairstyle_male_010")));

	//InteractBoxComponent = CreateDefaultSubobject<UACInteractableComponent>(TEXT("InteractBoxComponent"));
	//InteractBoxComponent->SetupAttachment(RootComponent);

	BlackMarketComponent = CreateDefaultSubobject<UACBlackMarketComponent>(TEXT("BlackMarketComponent"));

}

void AACTestBlackMarketDealer::OnInteracted(AACCharacter* InteractingPlayer)
{
	AC_LOG(LogSY, Log, TEXT("Dealer OnInteracted"));

	if (BlackMarketComponent)
	{
		BlackMarketComponent->OpenBlackMarket(InteractingPlayer);
	}

}

void AACTestBlackMarketDealer::BeginPlay()
{
	Super::BeginPlay();
	InteractBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AACTestBlackMarketDealer::OnInteractTriggerOverlapBegin);
	InteractBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AACTestBlackMarketDealer::OnInteractTriggerOverlapEnd);

	// 루트 컴포넌트의 로컬 바운드 가져오기
	FBoxSphereBounds RootBounds = RootComponent->CalcBounds(RootComponent->GetComponentTransform());
	// BoxExtent 설정 (약간 여유 포함)
	FVector Margin(50.f, 50.f, 50.f);
	InteractBoxComponent->SetBoxExtent(RootBounds.BoxExtent + Margin);
	// 박스 위치 루트에 맞추기
	InteractBoxComponent->SetRelativeLocation(FVector::ZeroVector);
}

void AACTestBlackMarketDealer::OnInteractTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AACTestMafiaCharacter* TestMafiaChar = Cast<AACTestMafiaCharacter>(OtherActor);
	if (TestMafiaChar == nullptr)
	{
		return;
	}
	AC_LOG(LogSY, Log, TEXT("InteractBox Begin"));
	TestMafiaChar->InteractDealer = this;
}

void AACTestBlackMarketDealer::OnInteractTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AACTestMafiaCharacter* TestMafiaChar = Cast<AACTestMafiaCharacter>(OtherActor);
	if (TestMafiaChar == nullptr)
	{
		return;
	}
	AC_LOG(LogSY, Log, TEXT("InteractBox End"));
	TestMafiaChar->InteractDealer = nullptr;
}