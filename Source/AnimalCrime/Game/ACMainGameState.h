// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACGameEnums.h"
#include "Character/ACCharacter.h"
#include "GameFramework/GameState.h"
#include "ACMainGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, float, NewScore);
/**
 *
 */
UCLASS()
class ANIMALCRIME_API AACMainGameState : public AGameState
{
	GENERATED_BODY()

#pragma region 생성자
public:
	AACMainGameState();
#pragma endregion

#pragma region 엔진 제공 함수
public:
	virtual void HandleBeginPlay() override;
	virtual void OnRep_ReplicatedHasBegunPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
#pragma endregion

public:
	UFUNCTION(Server, Reliable)
	void ServerChangeEscapeState(EEscapeState NewEscapeState);

#pragma region 관전
public:
	/**
		@brief 관전가능대상 폰 배열인 SpectatablePawns에 폰을 추가하는 함수
		@param Pawn - 관전 대상으로 추가할 폰
	**/
	void AddSpectatablePawn(APawn* Pawn);
	/**
		@brief SpectatablePawns에 폰을 삭제한다. 모든 클라이언트의 관전 대상이 사라졌는지 확인하고, 사라졌다면 다른 폰을 관전한다.
		@param Pawn - 관전 대상에서 삭제할 폰
	**/
	void RemoveSpectatablePawn(APawn* Pawn);

	FORCEINLINE const TArray<TObjectPtr<APawn>>& GetSpectatablePawns() const
	{
		return SpectatablePawns;
	}

#pragma endregion

public:
 /**
     @brief  PlayerArray에서 지정한 CharacterState을 가진 플레이어들의 PlayerState 목록을 반환한다.
     @param  Role - 조회할 플레이어의 역할
     @retval      - 지정한 CharacterState에 해당하는 AACPlayerState들의 배열
 **/
	TArray<TObjectPtr<class AACPlayerState>> GetPlayersByState(ECharacterState CharacterState) const;

#pragma region GameRuleManager와 동기화 및 테스트 함수
public:
	/**
		@brief UACGameRuleManager의 점수를 각 클라이언트에게 동기화 시키는 함수.
		@param InScore - 변경될 점수
	**/
	void UpdateTeamScore(float InScore);

	/**
		@brief  MainGameState가 가지고 있는 TeamScore를 반환하는 함수
		@retval  - 현재 TeamScore
	**/
	float GetTeamScore() const;
#pragma endregion

	UFUNCTION()
	void OnRep_TeamScore();

	UFUNCTION()
	float GetMaxScore() const { return MaxScore; }

	UFUNCTION()
	float GetMinScore() const { return MinScore; }

public:
	/**
		@brief 목적지를 등록하는 함수
		@param Actor - 목적지 Actor
	**/
	UFUNCTION(BlueprintCallable)
	void RegisterDestination(AActor* Actor);

	/**
		@brief 목적지 Actor의 정보를 반환하는 함수(변경 여지 있음)
		@retval  - 목적지 Actor
	**/
	UFUNCTION(BlueprintCallable)
	AActor* GetDestinationActor() const;
#pragma endregion

public:
	//!< 탈출 임무
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	EEscapeState EscapeState = EEscapeState::DeliverBomb;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<class AACBombInstallArea>> BombAreas;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<class AACEscapeArea>> EscapeAreas;

public:
	//!< 서버 전용. Replicated 안 함.
	UPROPERTY()
	TArray<TObjectPtr<class AACMafiaCharacter>> MafiaPlayers;

public:
	//!< 관전 가능한 폰 목록
	UPROPERTY(Replicated)
	TArray<TObjectPtr<APawn>> SpectatablePawns;

public:
	FOnScoreChanged OnScoreChanged;

private:
	UPROPERTY(ReplicatedUsing = OnRep_TeamScore)
	float TeamScore = 5000;

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float MaxScore = 7000;

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float MinScore = 0;

private:
	/** 목적지 정보 배열 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<class AActor>> DestinationObjects;
};
