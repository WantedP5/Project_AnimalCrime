// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ACHUDWidget.h"

#include "AnimalCrime.h"
#include "UI/Score/ACScoreWidget.h"
#include "UI/QuickSlot/ACQuickSlotWidget.h"
#include "Game/ACMainGameState.h"
#include "Game/ACPlayerState.h"
#include "UI/Money/ACMoneyWidget.h"
#include "Objects/MoneyData.h"
#include "Component/ACMoneyComponent.h"
#include "Character/ACCharacter.h"

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

	/*if (APlayerController* PC = GetOwningPlayer())
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
	}*/

	// PlayerState는 더 이상 돈을 관리하지 않음
	// MoneyComponent를 바인딩
	BindMoneyComponent();
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

void UACHUDWidget::BindMoneyComponent()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (APawn* Pawn = PC->GetPawn())
		{
			UACMoneyComponent* MoneyComp = Pawn->FindComponentByClass<UACMoneyComponent>();
			if (MoneyComp)
			{
				// 델리게이트 바인딩
				MoneyComp->OnMoneyChanged.AddDynamic(this, &UACHUDWidget::HandleMoneyChanged);

				// 초기값 설정
				HandleMoneyChanged(MoneyComp->GetMoney());

				UE_LOG(LogHG, Log, TEXT("MoneyComponent 바인딩 성공, 초기 돈: %d"), MoneyComp->GetMoney());
			}
			else
			{
				UE_LOG(LogHG, Warning, TEXT("MoneyComponent를 찾을 수 없음"));
			}
		}
	}
}
