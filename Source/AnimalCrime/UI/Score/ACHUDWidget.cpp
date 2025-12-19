// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Score/ACHUDWidget.h"

#include "ACScoreWidget.h"
#include "AnimalCrime.h"
#include "UI/HUD/ACQuickSlotWidget.h"
#include "Game/ACMainGameState.h"
#include "Game/ACPlayerState.h"
#include "UI/Money/ACMoneyWidget.h"

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

// 문제였음.
void UACHUDWidget::BindPlayerState()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		UE_LOG(LogHY, Error, TEXT("PC Success"));
		if (AACPlayerState* PS = PC->GetPlayerState<AACPlayerState>())
		{
			UE_LOG(LogHY, Error, TEXT("PS Success"));

			PS->OnMoneyChanged.AddDynamic(
				this,
				&UACHUDWidget::HandleMoneyChanged
			);

			HandleMoneyChanged(PS->GetMoney());
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

void UACHUDWidget::HandleMoneyChanged(int32 NewMoney)
{
	if (WBP_Money == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[UACHUDWidget::HandleMoneyChanged]: %d"), NewMoney);
	
	WBP_Money->UpdateMoney(NewMoney);
}
