// Fill out your copyright notice in the Description page of Project Settings.


#include "ACMainGameState.h"

#include "Net/UnrealNetwork.h"

#include "Character/ACMafiaCharacter.h"
#include "ACPlayerState.h"
#include "ACMainPlayerController.h"
#include "EscapeQuest/ACEscapeArea.h"
#include "Game/ACMainPlayerController.h"
#include "UI/ACHUDWidget.h"
#include "AnimalCrime.h"

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
	DOREPLIFETIME(AACMainGameState, EscapedCount);
	DOREPLIFETIME(AACMainGameState, SpectatablePawns);
}


void AACMainGameState::AddSpectatablePawn(APawn* Pawn)
{
	if (Pawn == nullptr)
	{
		return;
	}

	if (SpectatablePawns.Contains(Pawn) == true)
	{
		return;
	}

	SpectatablePawns.Add(Pawn);

	AC_LOG(LogSY, Log, TEXT("AddSpectatablePawn: %s"), *Pawn->GetName());
}

void AACMainGameState::RemoveSpectatablePawn(APawn* Pawn)
{
	if (Pawn == nullptr)
	{
		return;
	}

	if (SpectatablePawns.Remove(Pawn) > 0)
	{
		AC_LOG(LogSY, Log, TEXT("RemoveSpectatablePawn: %s"), *Pawn->GetName());

		// 모든 클라이언트에 직접 알림
		for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			AACMainPlayerController* PC = Cast<AACMainPlayerController>(It->Get());
			if (PC == nullptr)
			{
				continue;
			}

			//만약 삭제된 폰이 관전 대상이었으면 관전대상 변경
			PC->ClientNotifySpectateTargetRemoved(Pawn);
		}
	}
}

TArray<TObjectPtr<class AACPlayerState>> AACMainGameState::GetPlayersByLocation(ECharacterLocation CharacterLocation) const
{
	TArray<TObjectPtr<AACPlayerState>> Result;

	for (APlayerState* PS : PlayerArray)
	{
		AACPlayerState* ACPS = Cast<AACPlayerState>(PS);
		if (ACPS == nullptr)
		{
			continue;
		}

		//찾는 Location와 같으면 배열에 추가
		if (ACPS->CharacterLocation == CharacterLocation)
		{
			Result.Add(ACPS);
		}
	}

	return Result;
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
	for (auto InObject : DestinationObjects)
	{
		if (InObject == Actor)
		{
			AC_LOG(LogHY, Error, TEXT("Already In..."));
			return;
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
void AACMainGameState::OnRep_EscapedCount()
{
	OnEscapeCountChanged.Broadcast(EscapedCount);
}
void AACMainGameState::AddEscapedCount()
{
	++EscapedCount;
	OnRep_EscapedCount();
}
#pragma endregion

void AACMainGameState::Multicast_GlobalShowNotification_Implementation(const FText& Message, EPlayerRole TargetRole, ENotificationType NotificationType)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	APlayerController* PC = World->GetFirstPlayerController();
	if (PC == nullptr || PC->IsLocalController() == false)
	{
		return;
	}

	AACMainPlayerController* MainPC = Cast<AACMainPlayerController>(PC);
	if (MainPC == nullptr)
	{
		return;
	}

	AACPlayerState* PS = PC->GetPlayerState<AACPlayerState>();
	if (PS == nullptr)
	{
		return;
	}

	if(TargetRole != EPlayerRole::None && PS->PlayerRole != TargetRole)
	{
		return;
	}

	MainPC->ShowNotification(Message, NotificationType);
}

void AACMainGameState::GlobalShowNotification(const FText& Message, ENotificationType NotificationType)
{
	Multicast_GlobalShowNotification(Message, EPlayerRole::None, NotificationType);
}

TArray<class AACPlayerState*> AACMainGameState::GetPlayersByRoleAndLocation(EPlayerRole InRole, ECharacterLocation InLocation) const
{
	TArray<class AACPlayerState*> Result;

	for (APlayerState* PS : PlayerArray)
	{
		AACPlayerState* ACPS = Cast<AACPlayerState>(PS);
		if (ACPS == nullptr)
		{
			continue;
		}

		//찾는 State와 같으면 배열에 추가, None이면 모두 포함
		if(InRole != EPlayerRole::None && ACPS->PlayerRole != InRole)
		{
			continue;
		}
		if(InLocation != ECharacterLocation::None && ACPS->CharacterLocation != InLocation)
		{
			continue;
		}
		Result.Add(ACPS);
	}
	return Result;
}

void AACMainGameState::ServerChangeEscapeState_Implementation(EEscapeState NewEscapeState)
{
	// 마피아 플레이어들의 EscapeState 변경 (OnRep으로 클라이언트 UI 자동 업데이트)
	for (AACMafiaCharacter* Mafia : MafiaPlayers)
	{
		if (Mafia == nullptr)
		{
			continue;
		}

		AACPlayerState* PS = Mafia->GetPlayerState<AACPlayerState>();
		if (PS != nullptr)
		{
			PS->EscapeState = NewEscapeState;
			PS->OnRep_EscapeState();
		}

		// Escape 상태일 때 추가 처리
		if (NewEscapeState == EEscapeState::Escape)
		{
			Mafia->ClientSetBombHeld(false);
			Mafia->ClientSetEscapeAreaVisible(true);
		}
	}

	// Escape 상태일 때 서버에서만 콜리전 충돌 true로 설정
	if (NewEscapeState == EEscapeState::Escape)
	{
		for (AACEscapeArea* Area : EscapeAreas)
		{
			if (Area == nullptr)
			{
				continue;
			}
			Area->SetActorEnableCollision(true);
		}
	}
}
