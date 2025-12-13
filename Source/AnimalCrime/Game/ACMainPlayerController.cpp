
#include "ACMainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "ACTitlePlayerController.h"

AACMainPlayerController::AACMainPlayerController()
{
	//탈출 스크린 로드
	static ConstructorHelpers::FClassFinder<UUserWidget> EscapeScreenRef(TEXT("/Game/Project/UI/WBP_EscapeText.WBP_EscapeText_C"));
	if (EscapeScreenRef.Succeeded())
	{
		EscapeScreenClass = EscapeScreenRef.Class;
	}
}

void AACMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//ConsoleCommand(TEXT("show Collision"));
	ConsoleCommand(TEXT("Stat FPS"));
}

void AACMainPlayerController::ShowEscapeUI_Implementation()
{
	EscapeScreen = CreateWidget<UUserWidget>(this, EscapeScreenClass);
	if (EscapeScreen == nullptr)
	{
		return;
	}
	EscapeScreen->AddToViewport();
}


