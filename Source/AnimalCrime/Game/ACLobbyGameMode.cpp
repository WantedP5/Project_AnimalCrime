
#include "ACLobbyGameMode.h"
#include "ACLobbyPlayerController.h"
#include "ACLobbyGameState.h"
#include "Character/ACLobbyCharacter.h"
#include "Character/ACTestMafiaCharacter.h"
#include "AnimalCrime.h"

AACLobbyGameMode::AACLobbyGameMode()
{
	PlayerControllerClass = AACLobbyPlayerController::StaticClass();
	GameStateClass = AACLobbyGameState::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnBP(
		TEXT("/Game/Project/Character/BP_VoiceTestCharacter")
	);

	if (DefaultPawnBP.Succeeded())
	{
		DefaultPawnClass = DefaultPawnBP.Class;
	}
	//DefaultPawnClass = AACLobbyCharacter::StaticClass();
}

//void AACLobbyGameMode::PostLogin(APlayerController* NewPlayer)
//{
//	Super::PostLogin(NewPlayer);
//	RestartPlayer(NewPlayer);
//	AACLobbyGameState* GS = GetGameState<AACLobbyGameState>();
//	if (GS == nullptr)
//	{
//		return;
//	}
//	AC_LOG(LogSY, Log, TEXT("PlayerLogIn"));
//	GS->UpdateReadyPlayer();
//}
//
//void AACLobbyGameMode::Logout(AController* Exiting)
//{
//	Super::Logout(Exiting);
//
//	AACLobbyGameState* GS = GetGameState<AACLobbyGameState>();
//	if (GS == nullptr)
//	{
//		return;
//	}
//
//	GS->UpdateReadyPlayer();
//
//}
