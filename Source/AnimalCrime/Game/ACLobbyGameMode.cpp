
#include "ACLobbyGameMode.h"
#include "ACLobbyPlayerController.h"
#include "ACLobbyGameState.h"
#include "Character/ACLobbyCharacter.h"
#include "Character/ACMafiaCharacter.h"
#include "ACAdvancedFriendsGameInstance.h"
#include "ACPlayerState.h"
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
	const int32 PoliceIndex = FMath::RandRange(0, PlayerCount - 1);

	for (int32 i = 0; i < PlayerCount; ++i)
	{
		AACPlayerState* PS = Cast<AACPlayerState>(GameState->PlayerArray[i]);

		if (PS == nullptr || PS->GetUniqueId().IsValid() == false)
		{
			continue;
		}

		const EPlayerRole AssignedRole = (i == PoliceIndex) ? EPlayerRole::Police : EPlayerRole::Mafia;

		// 플레이어 역할 지정
		PS->PlayerRole = AssignedRole;

		// 플레이어 스테이트 백업
		GI->SavedPlayerRoles.Add(PS->GetUniqueId(), AssignedRole);

		AC_LOG(LogSY, Log, TEXT("%s : %s"), *PS->GetPlayerName(), *PS->GetUniqueId()->ToString());
	}


	GI->LoadGameMap();


	AC_LOG(LogSY, Log, TEXT("맵이동"));	
}
