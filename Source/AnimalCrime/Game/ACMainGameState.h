// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACGameEnums.h"
#include "GameFramework/GameState.h"
#include "ACMainGameState.generated.h"

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
	//UFUNCTION(Server, Reliable)
	//void ChangeEscapeState(EEscapeState NewEscapeState);

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 TeamScore = 0;

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
};
