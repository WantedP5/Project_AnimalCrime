
#include "ACMainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "ACTitlePlayerController.h"
#include "UI/Score/ACHUDWidget.h"

AACMainPlayerController::AACMainPlayerController()
{
	//탈출 스크린 로드
	static ConstructorHelpers::FClassFinder<UUserWidget> EscapeScreenRef(TEXT("/Game/Project/UI/WBP_EscapeText.WBP_EscapeText_C"));
	if (EscapeScreenRef.Succeeded())
	{
		EscapeScreenClass = EscapeScreenRef.Class;
	}
	
	// HUD Class 대입.
	static ConstructorHelpers::FClassFinder<UACHUDWidget> ACHUDWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Project/UI/GameScore/WBP_ACHUD.WBP_ACHUD_C'"));
	if (ACHUDWidgetRef.Succeeded())
	{
		ACHUDWidgetClass = ACHUDWidgetRef.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACHUDWidgetRef is empty"));
	}
}

void AACMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//ConsoleCommand(TEXT("show Collision"));
	ConsoleCommand(TEXT("Stat FPS"));
	UE_LOG(LogTemp, Warning, TEXT("AACMainPlayerController BeginPlay"));
	
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	if (!IsLocalController())
	{
		return;   // 🔥 이 줄이 핵심
	}
	ACHUDWidget = CreateWidget<UACHUDWidget>(this, ACHUDWidgetClass);
	if (ACHUDWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ACHUDWidget is empty"));
		return ;
	}
	
	// 뭐하는 함수인지 알아보기.
	ACHUDWidget->AddToViewport();
	
	ACHUDWidget->BindGameState();
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
