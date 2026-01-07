#include "ACBombInstallArea.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "Item/ACEscapeMissionBomb.h"

#include "Game/ACMainGameState.h"
#include "AnimalCrime.h"

void AACBombInstallArea::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AACBombInstallArea::OnBombTriggerOverlapBegin);

	AACMainGameState* GS = GetWorld()->GetGameState<AACMainGameState>();
	if (GS == nullptr)
	{
		AC_LOG(LogSY, Log, TEXT("GS is nullptr"));
		return;
	}

	AC_LOG(LogSY, Log, TEXT("BombAreas add"));
	GS->BombAreas.Add(this); // 클라만 저장
}

void AACBombInstallArea::OnBombTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() == false) //서버만 처리
	{
		return;
	}
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

	//1초 후에 폭탄 삭제
	FTimerHandle BombDestroyHandle;

	GetWorld()->GetTimerManager().SetTimer(
		BombDestroyHandle,
		FTimerDelegate::CreateUObject(
			this,
			&AACBombInstallArea::OnBombDestroyComplete,
			OverlappedBomb
		),
		1.0f,
		false
	);
}

void AACBombInstallArea::OnBombDestroyComplete(AACEscapeMissionBomb* Bomb)
{
	if (HasAuthority() == false)
	{
		return;
	}

	if (IsValid(Bomb) == false)
	{
		return;
	}
	Bomb->Multicast_PlayExplosion();
	Bomb->SetLifeSpan(0.2f);

	AACMainGameState* GS = GetWorld()->GetGameState<AACMainGameState>();
	if (GS == nullptr)
	{
		return;
	}
	GS->ServerChangeEscapeState(EEscapeState::Escape);
}
