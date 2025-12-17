// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACGameEnums.h"
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
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UFUNCTION(Server, Reliable)
	void ServerChangeEscapeState(EEscapeState NewEscapeState);

public:
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
	float GetMaxScore() const{return MaxScore;}
	
	UFUNCTION()
	float GetMinScore() const{return MinScore;}
	
public:
	UFUNCTION(BlueprintCallable)
	void RegisterDestination(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	AActor* GetDestinationActor() const;
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
	TArray<TObjectPtr<class AACTestMafiaCharacter>> MafiaPlayers;
	
public:
	FOnScoreChanged OnScoreChanged;
	
private:
	UPROPERTY(ReplicatedUsing=OnRep_TeamScore)
	float TeamScore = 5000;
	
	UPROPERTY(Replicated, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float MaxScore = 7000;
	
	UPROPERTY(Replicated, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float MinScore = 0;
	
private:
	/** 목적지 정보 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TArray<TObjectPtr<class AActor>> DestinationObjects;
};
