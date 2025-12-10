#include "ACBombInstallArea.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "Item/ACEscapeMissionBomb.h"

#include "AnimalCrime.h"

void ACBombInstallArea::BeginPlay()
{
	Super::BeginPlay();

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

	//2초 후에 폭탄 삭제
	FTimerHandle BombDestroyHandle;
	GetWorld()->GetTimerManager().SetTimer(
		BombDestroyHandle,
		FTimerDelegate::CreateLambda([OverlappedBomb]()
			{
				if (IsValid(OverlappedBomb))
				{
					OverlappedBomb->Destroy();
				}
			}),
		1.0f, false
	);

}
