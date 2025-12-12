
#include "ACMainPlayerController.h"

AACMainPlayerController::AACMainPlayerController()
{
}

void AACMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//ConsoleCommand(TEXT("show Collision"));
	ConsoleCommand(TEXT("Stat FPS"));
}
