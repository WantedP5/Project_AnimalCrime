
#include "ACLobbyGameMode.h"
#include "ACLobbyPlayerController.h"
#include "ACLobbyGameState.h"
#include "Character/ACLobbyCharacter.h"
#include "Character/ACMafiaCharacter.h"
#include "ACAdvancedFriendsGameInstance.h"
#include "ACPlayerState.h"
#include "Algo/RandomShuffle.h"
#include "AnimalCrime.h"

AACLobbyGameMode::AACLobbyGameMode()
{
	PlayerControllerClass = AACLobbyPlayerController::StaticClass();
	GameStateClass = AACLobbyGameState::StaticClass();
	PlayerStateClass = AACPlayerState::StaticClass();
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnBP(
		TEXT("/Game/Project/Character/BP_VoiceTestCharacter")
	);

	//if (DefaultPawnBP.Succeeded())
	//{
	//	DefaultPawnClass = DefaultPawnBP.Class;
	//}

	DefaultPawnClass = AACLobbyCharacter::StaticClass();
	bUseSeamlessTravel = true;
}
void AACLobbyGameMode::StartGamePlay()
{
	//서버에서만 한 번 실행
	if (!HasAuthority())
	{
		return;
	}

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	if (GameState == nullptr || GameState->PlayerArray.Num() == 0)
	{
		return;
	}

	UACAdvancedFriendsGameInstance* GI = GetGameInstance<UACAdvancedFriendsGameInstance>();
	if (GI == nullptr)
	{
		return;
	}

	//역할 분배

	//기존 플레이어 역할 데이터 삭제
	GI->SavedPlayerRoles.Empty();

	const int32 PlayerCount = GameState->PlayerArray.Num();
	
	// 인원 결정
	// 1인 : 마피아 1명
	// 2~3인 : 마피아 1~2명, 경찰 1명
	// 4인 이상 : 마피아 2~N명, 경찰 2명
	int32 NumPolice;
	if (PlayerCount <= 1)
	{
		NumPolice = 0;
	}
	else if (PlayerCount >1 && PlayerCount <= 3)
	{
		NumPolice = 1;
	}
	else
	{
		NumPolice = 2;
	}
	
	// 0 ~ PlayerCount-1 인덱스 셔플
	TArray<int32> Indices;
	Indices.Reserve(PlayerCount);
	for (int32 i = 0; i < PlayerCount; ++i)
	{
		Indices.Add(i);
	}
	Algo::RandomShuffle(Indices);

	// 앞에서 NumPolice 만큼 경찰로 지정
	TSet<int32> PoliceIndices;
	for (int32 i = 0; i < NumPolice; ++i)
	{
		PoliceIndices.Add(Indices[i]);
	}

	for (int32 i = 0; i < PlayerCount; ++i)
	{
		AACPlayerState* PS = Cast<AACPlayerState>(GameState->PlayerArray[i]);

		if (PS == nullptr || PS->GetUniqueId().IsValid() == false)
		{
			continue;
		}

		const EPlayerRole AssignedRole = PoliceIndices.Contains(i) ? EPlayerRole::Police : EPlayerRole::Mafia;

		// 플레이어 역할 지정
		PS->PlayerRole = AssignedRole;
		// 플레이어 스테이트 백업
		GI->SavedPlayerRoles.Add(PS->GetUniqueId(), AssignedRole);

		AC_LOG(LogSY, Log, TEXT("%s : %s -> %s"), *PS->GetPlayerName(), *PS->GetUniqueId()->ToString(),
			AssignedRole == EPlayerRole::Police ? TEXT("Police") : TEXT("Mafia"));
	}

	AC_LOG(LogSY, Log, TEXT("맵이동"));

	GI->LoadGameMap();
}
