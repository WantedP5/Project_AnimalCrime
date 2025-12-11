#include "EscapeQuest/ACBlackMarketComponent.h"
#include "Item/ACEscapeMissionBomb.h"
#include "Character/ACTestMafiaCharacter.h"
#include "Game/ACMainGameState.h"

#include "AnimalCrime.h"

UACBlackMarketComponent::UACBlackMarketComponent()
{

}

void UACBlackMarketComponent::OpenBlackMarket(AACCharacter* InteractingPlayer)
{
	AC_SUBLOG(LogSY, Log, TEXT("BlackMarket Opened"));
	if (InteractingPlayer == nullptr)
	{
		AC_SUBLOG(LogSY, Warning, TEXT("InteractingPlayer is nullptr"));
		return;
	}
	AACTestMafiaCharacter* Mafia = Cast<AACTestMafiaCharacter>(InteractingPlayer);
	if (Mafia == nullptr)
	{
		AC_SUBLOG(LogSY, Log, TEXT("Character is not Mafia"));
		return;
	}

	// 서버에서만 실행되도록 보호
	if (GetOwner() == nullptr || GetOwner()->HasAuthority() == false)
	{
		AC_SUBLOG(LogSY, Warning, TEXT("OpenBlackMarket called on non-server"));
		return;
	}

	AACMainGameState* GS = GetWorld()->GetGameState<AACMainGameState>();
	if (GS == nullptr)
	{
		AC_SUBLOG(LogSY, Warning, TEXT("GameState is nullptr"));
		return;
	}
	if (GS->EscapeState != EEscapeState::DeliverBomb)
	{
		AC_SUBLOG(LogSY, Log, TEXT("EscapeState is not DeliverBomb"));
		return;
	}

	if (Mafia->HandBomb != nullptr)
	{
		AC_SUBLOG(LogSY, Log, TEXT("Already Hand has Bomb"));
	}

	else
	{
		AC_SUBLOG(LogSY, Log, TEXT("Get Bomb!!"));
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		// 폭탄 스폰
		AACEscapeMissionBomb* SpawnedBomb = GetWorld()->SpawnActor<AACEscapeMissionBomb>
			(AACEscapeMissionBomb::StaticClass(),
				InteractingPlayer->GetActorLocation(),
				FRotator::ZeroRotator,
				Params
			);

		SpawnedBomb->AttachedCharacter = Mafia;

		//아이템 핸드로 이동
		Mafia->HandBomb = SpawnedBomb;

		//폭탄 설치 가능 구역 보이게 하기
		Mafia->ClientSetBombAreaVisible(true);
		//캐릭터에 폭탄 부착
		SpawnedBomb->AttachToCharacter();
	}
}


void UACBlackMarketComponent::BeginPlay()
{
	Super::BeginPlay();


}


