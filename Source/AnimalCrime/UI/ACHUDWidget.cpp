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
	// ※ 확인 했으면 위에 주석들 다 지워주세요
	BindMoneyComponent();
}

void UACHUDWidget::HandleScoreChanged(float NewScore)
{
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
	
	WBP_Money->UpdateMoney(NewMoney);
}

void UACHUDWidget::BindMoneyComponent()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		APawn* Pawn = PC->GetPawn();
		if (Pawn == nullptr)
		{
			// Pawn이 아직 없으면 타이머로 재시도
			UE_LOG(LogHG, Warning, TEXT("Pawn이 없음, 0.1초 후 재시도"));

			FTimerHandle RetryTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				RetryTimerHandle,
				this,
				&UACHUDWidget::BindMoneyComponent,
				0.1f,  // 0.1초 후
				false  // 한 번만
			);
			return;
		}

		UACMoneyComponent* MoneyComp = Pawn->FindComponentByClass<UACMoneyComponent>();
		if (MoneyComp)
		{
			// 이미 바인딩되어 있는지 확인 (중복 방지)
			if (MoneyComp->OnMoneyChanged.Contains(this, FName("HandleMoneyChanged")))
			{
				UE_LOG(LogHG, Warning, TEXT("이미 바인딩됨, 스킵"));
				return;
			}

			// 델리게이트 바인딩
			MoneyComp->OnMoneyChanged.AddDynamic(this, &UACHUDWidget::HandleMoneyChanged);

			// 초기값 설정
			HandleMoneyChanged(MoneyComp->GetMoney());
		}
		else
		{
			UE_LOG(LogHG, Error, TEXT("MoneyComponent를 찾을 수 없음"));
		}
	}
}
