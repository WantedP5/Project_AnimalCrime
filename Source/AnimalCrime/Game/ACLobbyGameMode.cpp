
#include "ACLobbyGameMode.h"
#include "ACLobbyPlayerController.h"
#include "Character/ACTestMafiaCharacter.h"

AACLobbyGameMode::AACLobbyGameMode()
{
	PlayerControllerClass = AACLobbyPlayerController::StaticClass();

    static ConstructorHelpers::FClassFinder<APawn> DefaultPawnBP(
        TEXT("/Game/Project/Character/BP_VoiceTestCharacter")
    );

    if (DefaultPawnBP.Succeeded())
    {
        DefaultPawnClass = DefaultPawnBP.Class;
    }

	//DefaultPawnClass = AACTestMafiaCharacter::StaticClass();
}
