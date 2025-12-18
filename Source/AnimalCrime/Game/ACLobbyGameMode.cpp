
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

	if (DefaultPawnBP.Succeeded())
	{
		DefaultPawnClass = DefaultPawnBP.Class;
	}

	bUseSeamlessTravel = true;
	//DefaultPawnClass = AACLobbyCharacter::StaticClass();
}
void AACLobbyGameMode::StartGamePlay()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	if (GameState == nullptr || GameState->PlayerArray.Num() == 0)
	{
		return;
	}

	// 랜덤 1명만 경찰 인덱스
	const int32 PoliceIndex = FMath::RandRange(0, GameState->PlayerArray.Num() - 1);

	for (int32 i = 0; i < GameState->PlayerArray.Num(); ++i)
	{
		AACPlayerState* ACPS = Cast<AACPlayerState>(GameState->PlayerArray[i]);
		if (ACPS == nullptr)
		{
			continue;
		}
		ACPS->CharacterType = (i == PoliceIndex) ? EACCharacterType::Police : EACCharacterType::Mafia;
	}

	UACAdvancedFriendsGameInstance* GI = GetGameInstance<UACAdvancedFriendsGameInstance>();
	if (GI == nullptr)
	{
		return;
	}
	GI->LoadGameMap();


	AC_LOG(LogSY, Log, TEXT("맵이동"));	
}
