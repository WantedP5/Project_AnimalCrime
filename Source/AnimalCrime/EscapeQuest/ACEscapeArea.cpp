
#include "EscapeQuest/ACEscapeArea.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"

#include "Game/ACMainGameState.h"
#include "AnimalCrime.h"
void AACEscapeArea::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AACEscapeArea::OnEscapeOverlapBegin);

	AACMainGameState* GS = GetWorld()->GetGameState<AACMainGameState>();
	if (GS == nullptr)
	{
		AC_LOG(LogSY, Log, TEXT("GS is nullptr"));
		return;
	}

	AC_LOG(LogSY, Log, TEXT("BombAreas add"));
	GS->EscapeAreas.Add(this); // 클라만 저장
}

void AACEscapeArea::OnEscapeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

