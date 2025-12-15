#include "ACTitleGameMode.h"
#include "ACTitlePlayerController.h"

AACTitleGameMode::AACTitleGameMode()
{
	PlayerControllerClass = AACTitlePlayerController::StaticClass();
}
