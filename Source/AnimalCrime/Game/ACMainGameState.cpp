// Fill out your copyright notice in the Description page of Project Settings.


#include "ACMainGameState.h"

#include "AnimalCrime.h"
#include "Net/UnrealNetwork.h"

#include "Character/ACMafiaCharacter.h"

#pragma region 생성자
AACMainGameState::AACMainGameState()
{
}

void AACMainGameState::HandleBeginPlay()
{
	AC_LOG(LogHY, Warning, TEXT("Begin"));
	Super::HandleBeginPlay();
	AC_LOG(LogHY, Warning, TEXT("End"));
}

void AACMainGameState::OnRep_ReplicatedHasBegunPlay()
{
	AC_LOG(LogHY, Warning, TEXT("Begin"));
	Super::OnRep_ReplicatedHasBegunPlay();
	AC_LOG(LogHY, Warning, TEXT("End"));
}
#pragma endregion

#pragma region 엔진 제공 함수
void AACMainGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AACMainGameState, TeamScore);
	DOREPLIFETIME(AACMainGameState, EscapeState);
}

#pragma region GameRuleManager와 동기화 및 테스트 함수
void AACMainGameState::UpdateTeamScore(float InScore)
{
	TeamScore = InScore;
}

float AACMainGameState::GetTeamScore() const
{
	return TeamScore;
}

void AACMainGameState::OnRep_TeamScore()
{
	UE_LOG(LogTemp, Error, TEXT("OnRep_Score 호출 :%f"), TeamScore);
	
	OnScoreChanged.Broadcast(TeamScore);
}
#pragma endregion

void AACMainGameState::RegisterDestination(AActor* Actor)
{
	for (auto InObject:DestinationObjects)
	{
		if (InObject == Actor)
		{
			AC_LOG(LogHY, Error, TEXT("Already In..."));
			return ;
		}
	}
	
	// 객체 추가.
	DestinationObjects.Add(Actor);
}

AActor* AACMainGameState::GetDestinationActor() const
{
	if (DestinationObjects.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("DestinationObjects가 존재하지 않습니다."));
		return nullptr;
	}
	
	// @InComplete
	// @Todo 현재 랜덤 값이지만, 나중에는 최대 인원수를 둬야할 것 같음.
	int32 RandIndex = FMath::RandRange(0, DestinationObjects.Num() - 1);
	return DestinationObjects[RandIndex];
}
#pragma endregion

void AACMainGameState::ServerChangeEscapeState_Implementation(EEscapeState NewEscapeState)
{
	EscapeState = NewEscapeState;

	switch (EscapeState)
	{
	case EEscapeState::GameStart:
		break;
	case EEscapeState::DeliverBomb:
		break;
	case EEscapeState::Escape:

		//마피아들의 폭탄설치가능구역 Visible 끄기
		for (AACMafiaCharacter* Mafia : MafiaPlayers)
		{
			if (Mafia == nullptr)
			{
				continue;
			}
			Mafia->ClientSetBombAreaVisible(false);
			Mafia->ClientSetEscapeAreaVisible(true);
		}
		break;
	default:
		break;
	}
}
