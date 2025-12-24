// Fill out your copyright notice in the Description page of Project Settings.


#include "ACGameRuleManager.h"

#include "ACAdvancedFriendsGameInstance.h"
#include "ACMainGameInstance.h"
#include "ACMainGameMode.h"
#include "ACMainGameState.h"
#include "Kismet/GameplayStatics.h"
#include "ACPlayerState.h"
#include "AnimalCrime.h"

UACGameRuleManager::UACGameRuleManager()
{
}

void UACGameRuleManager::Init(AACMainGameMode* InGameMode)
{
	GameMode = Cast<AACMainGameMode>(InGameMode);
	GameScoreGauge = 5000.0f;
}

AACMainGameMode* UACGameRuleManager::GetOwner() const
{
	return GameMode;
}

float UACGameRuleManager::GetScoreGauge() const
{
	return GameScoreGauge;
}

void UACGameRuleManager::OnObjectDestroyed(float InScore)
{
	// 스코어가 음수인 경우는 불가능
	if (InScore <= 0)
	{
		return ;
	}
	
	AACMainGameState* MainGameState = GetOwner()->GetGameState<AACMainGameState>();
	if (MainGameState == nullptr)
	{
		return;
	}
	
	GameScoreGauge -= InScore;
	MainGameState->UpdateTeamScore(GameScoreGauge);
	if (GetOwner()->HasAuthority())
	{
		MainGameState->OnRep_TeamScore();
	}
	if (GameScoreGauge <= MafiaWinThreshold)
	{
		UWorld* World = GameMode->GetWorld();
		if (World && GameMode->HasAuthority())
		{
			World->GetTimerManager().SetTimer(TimerHandle_NextMap, this, &UACGameRuleManager::LoadNextMap, 10.0f, false);
		}
	}
}

void UACGameRuleManager::OnAttackCitizen(float InScore)
{
	// 스코어가 음수인 경우는 불가능
	if (InScore <= 0)
	{
		return;
	}
	
	GameScoreGauge -= InScore;
	if (GameScoreGauge <= MafiaWinThreshold)
	{
		// 미구현
		UE_LOG(LogTemp, Log, TEXT("어머나 마피아 승"));
	}
}

void UACGameRuleManager::CheckGameEndCondition()
{
	UE_LOG(LogSY, Log, TEXT("게임종료 판단"));

	if (GetOwner() == nullptr)
	{
		UE_LOG(LogSY, Log, TEXT("GameMode가 nullptr 이어서 게임 종료 판단 불가능"));
		return;
	}

	AACMainGameState* GS = GetOwner()->GetGameState<AACMainGameState>();
	if (GS == nullptr)
	{
		return;
	}

	int32 MafiaNum = 0;
	int32 PrisonNum = 0;
	int32 EscapeNum = 0;


	for (APlayerState* PS : GS->PlayerArray)
	{
		AACPlayerState* ACPS = Cast<AACPlayerState>(PS);
		if (ACPS == nullptr)
		{
			continue;
		}

		//찾는 State와 같으면 배열에 추가
		if (ACPS->PlayerRole == EPlayerRole::Mafia)
		{
			++MafiaNum;
		}
		if (ACPS->CharacterState == ECharacterState::Prison)
		{
			++PrisonNum;
		}
		else if (ACPS->CharacterState == ECharacterState::Escape)
		{
			++EscapeNum;
		}
	}

	UE_LOG(LogSY, Log, TEXT("총 마피아 수: %d 감옥: %d 탈출: %d"), MafiaNum, PrisonNum, EscapeNum);

	//모든 마피아가 감옥 또는 탈출 상태면 게임 종료
	if (MafiaNum == PrisonNum + EscapeNum)
	{
		LoadNextMap();
	}
}

void UACGameRuleManager::RewardPoliceForAction(EPoliceAction Action, float InScore)
{
	switch (Action)
	{
	case EPoliceAction::DestroyObject:
	{
		OnObjectDestroyed(InScore);
		break;
	}
	case EPoliceAction::AttackCivilian:
	{
		OnAttackCitizen(InScore);
		break;
	}
	default:
	{
		break;
	}
	}
}

void UACGameRuleManager::RewardMafiaForAction(EMafiaAction Action, float InScore)
{
	switch (Action)
	{
	case EMafiaAction::DestroyObject:
		{
			OnObjectDestroyed(InScore);
			break;
		}
	case EMafiaAction::AttackCivilian:
		{
			OnAttackCitizen(InScore);
			break;
		}
	default:
		{
			break;
		}
	}
}

void UACGameRuleManager::HandleVictory()
{
	// 승리 처리 (UI 표시, 로그 등)
	UE_LOG(LogTemp, Warning, TEXT("%s 승리!"), TEXT("누군가"));

	// 10초 뒤 다음 맵 로드 예약
	GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle_NextMap, this, &UACGameRuleManager::LoadNextMap, 10.0f, false);
}

void UACGameRuleManager::LoadNextMap()
{
	// @Todo: World 체크해야할 수도...
	
	UACAdvancedFriendsGameInstance* GameInstance = GetOwner()->GetWorld()->GetGameInstance<UACAdvancedFriendsGameInstance>();
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s 승리!"), TEXT("?????"));
		return;
	}
	
	GameInstance->LoadLobbyMap();
}
