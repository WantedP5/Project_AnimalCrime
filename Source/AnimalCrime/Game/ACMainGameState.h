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
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UFUNCTION(Server, Reliable)
	void ServerChangeEscapeState(EEscapeState NewEscapeState);

public:
public:
	FOnScoreChanged OnScoreChanged;
	
	UPROPERTY(ReplicatedUsing=OnRep_TeamScore)
	float TeamScore = 5000;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	float MaxScore = 7000;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	float MinScore = 0;

	UFUNCTION()
	void OnRep_TeamScore();
	
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
	
private:
	
	
	
	/** 목적지 정보 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TArray<TObjectPtr<class AActor>> DestinationObjects;
};
