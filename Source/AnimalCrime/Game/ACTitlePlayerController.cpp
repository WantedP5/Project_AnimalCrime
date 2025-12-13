// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ACTitlePlayerController.h"
#include "UI/GameStart/ACGameStartScreen.h"

AACTitlePlayerController::AACTitlePlayerController()
{
	//GameStart 스크린 로드
	static ConstructorHelpers::FClassFinder<UACGameStartScreen> GameStartScreenRef(TEXT("/Game/Project/UI/GameStart/WBP_GameStartScreen.WBP_GameStartScreen_C"));
	if (GameStartScreenRef.Succeeded())
	{
		GameStartScreenClass = GameStartScreenRef.Class;
	}
}

void AACTitlePlayerController::BeginPlay()
{
	Super::BeginPlay();
	GameStartScreen = CreateWidget<UACGameStartScreen>(this, GameStartScreenClass);
	if (GameStartScreen == nullptr)
	{
		return;
	}
	GameStartScreen->AddToViewport();
}
