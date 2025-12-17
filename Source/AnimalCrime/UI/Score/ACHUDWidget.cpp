// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Score/ACHUDWidget.h"

#include "ACScoreWidget.h"
#include "Game/ACMainGameState.h"

void UACHUDWidget::BindGameState()
{
	if (UWorld* World = GetWorld())
	{
		if (AACMainGameState* GS = World->GetGameState<AACMainGameState>())
		{
			GS->OnScoreChanged.AddDynamic(this, &UACHUDWidget::HandleScoreChanged);
			
			HandleScoreChanged(GS->GetTeamScore());
		}
	}
}

void UACHUDWidget::HandleScoreChanged(float NewScore)
{
	const float Percent = FMath::Clamp(NewScore / 7000.f,
		0.f,
		1.f
	);
	if (WBP_Score == nullptr)
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		if (AACMainGameState* MainGameState = World->GetGameState<AACMainGameState>())
		{
			WBP_Score->UpdateScore(NewScore, MainGameState->GetMaxScore());
		}
	}

}