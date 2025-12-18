
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
	DOREPLIFETIME(AACLobbyGameState, ReadyPlayerArray);
}

void AACLobbyGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (PlayerState == nullptr)
	{
		return;
	}
	//호스트는 시작부터 레디플레이어에 추가
	if (HasAuthority() == true && IsHostPlayer(PlayerState) == true)
	{
		ReadyPlayerArray.AddUnique(PlayerState); //AddUnique는 중복없이 하나만 추가해주는 함수
	}

	UpdateReadyPlayer();
}

void AACLobbyGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	if (HasAuthority() && PlayerState)
	{
		// 로그아웃 시 Ready 상태였다면 제거
		ReadyPlayerArray.Remove(PlayerState);
	}
	UpdateReadyPlayer();
}

void AACLobbyGameState::UpdateReadyPlayer()
{
	if (HasAuthority() == false)
	{
		return;
	}

	// 람다함수를 사용해 비정상적으로 종료된 유효하지 않은 PlayerState 정리
	ReadyPlayerArray.RemoveAll([](APlayerState* PS)
		{
			//true면 삭제함.
			return IsValid(PS) == false;
		});

	//PlayerCount 갱신
	AllPlayerCount = PlayerArray.Num();

	ReadyPlayerCount = ReadyPlayerArray.Num();

	if (HasAuthority())
	{
		OnRep_PlayerCount(); //리슨서버의 서버는 자동으로 OnRep_PlayerCount()이 호출안된다. 수동으로 호출
	}
}

void AACLobbyGameState::SetReadyPlayer(APlayerState* PS, bool bReady)
{
	if (!HasAuthority() || PS == nullptr)
	{
		return;
	}

	if (bReady == true)
	{
		ReadyPlayerArray.AddUnique(PS);
	}
	else
	{
		ReadyPlayerArray.Remove(PS);
	}

	UpdateReadyPlayer();
}

bool AACLobbyGameState::IsHostPlayer(APlayerState* PS)
{
	if (PS == nullptr)
	{
		return false;
	}

	AController* Controller = Cast<AController>(PS->GetOwner());
	if (Controller == nullptr)
	{
		return false;
	}

	return Controller->IsLocalController() == true && Controller->HasAuthority() == true;
}

void AACLobbyGameState::OnRep_PlayerCount()
{
	ReadyPlayerCount = ReadyPlayerArray.Num();
	OnPlayerCountChanged.Broadcast(ReadyPlayerCount, AllPlayerCount);
}
