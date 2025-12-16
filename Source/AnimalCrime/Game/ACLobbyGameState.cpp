
#include "Game/ACLobbyGameState.h"
#include "Game/ACLobbyPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

#include "AnimalCrime.h"
void AACLobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AACLobbyGameState, ReadyPlayerCount);
	DOREPLIFETIME(AACLobbyGameState, AllPlayerCount);
}

void AACLobbyGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	UpdateReadyPlayer();
}

void AACLobbyGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	UpdateReadyPlayer();
}

void AACLobbyGameState::UpdateReadyPlayer()
{
	AC_LOG(LogSY, Log, TEXT("UpdateReady Player %d, %d"), AllPlayerCount, PlayerArray.Num());
	if (AllPlayerCount == PlayerArray.Num())
	{
		return;
	}

	AC_LOG(LogSY, Log, TEXT("UpdateReady Player2 %d, %d"), AllPlayerCount, PlayerArray.Num());
	//PlayerCount 갱신
	AllPlayerCount = PlayerArray.Num();
	if (HasAuthority())
	{
		OnRep_PlayerCount(); //리슨서버의 서버는 자동으로 OnRep_PlayerCount()이 호출안된다. 수동으로 호출
	}
}

void AACLobbyGameState::OnRep_PlayerCount()
{
	AC_LOG(LogSY, Log, TEXT("UpdateReady Player3 %d, %d"), AllPlayerCount, PlayerArray.Num());
	OnPlayerCountChanged.Broadcast(ReadyPlayerCount,AllPlayerCount);
}
