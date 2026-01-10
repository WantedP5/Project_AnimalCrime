
#include "UI/GameResult/ACGameResultScreen.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "ACGameResultWinnerBox.h"
#include "Game/ACMainGameState.h"
#include "AnimalCrime.h"

void UACGameResultScreen::NativeConstruct()
{
	Super::NativeConstruct();

	if (FadeInAnim == nullptr)
	{
		return;
	}

	PlayAnimation(FadeInAnim);
}

void UACGameResultScreen::SetGameResult(EGameEndType GameEndType)
{
	if (GameResultText == nullptr)
	{
		UE_LOG(LogSY, Log, TEXT("GameResultText is nullptr"));
		return;
	}

	AACMainGameState* GS = GetWorld()->GetGameState<AACMainGameState>();
	if (GS == nullptr)
	{
		UE_LOG(LogSY, Log, TEXT("GameState is nullptr"));
		return;
	}

	TArray< class AACPlayerState*> Winners;

	if (GameEndType == EGameEndType::AllPrison || GameEndType == EGameEndType::TimeOver)
	{
		GameResultText->SetText(FText::FromString(TEXT("경찰 승리")));
		if(WinnerBoxClass==nullptr || WinnerBoxes==nullptr)
		{
			UE_LOG(LogSY, Log, TEXT("WinnerBoxClass or WinnerBoxes is nullptr"));
			return;
		}
		Winners = GS->GetPlayersByRoleAndLocation(EPlayerRole::Police, ECharacterLocation::None);
	}
	else
	{
		GameResultText->SetText(FText::FromString(TEXT("마피아 승리")));
		Winners = GS->GetPlayersByRoleAndLocation(EPlayerRole::Mafia, ECharacterLocation::None);
	}

	for (AACPlayerState* PS : Winners)
	{
		UACGameResultWinnerBox* WinnerBox = CreateWidget<UACGameResultWinnerBox>(GetWorld(), WinnerBoxClass);
		if (WinnerBox == nullptr)
		{
			UE_LOG(LogSY, Log, TEXT("WinnerBox is nullptr"));
			continue;
		}
		WinnerBox->SetWinnerInfo(PS);
		WinnerBoxes->AddChild(WinnerBox);
	}
}
