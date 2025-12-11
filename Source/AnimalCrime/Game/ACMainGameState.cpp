// Fill out your copyright notice in the Description page of Project Settings.


#include "ACMainGameState.h"

#include "Net/UnrealNetwork.h"

#include "Character/ACTestMafiaCharacter.h"

void AACMainGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AACMainGameState, TeamScore);
	DOREPLIFETIME(AACMainGameState, EscapeState);
}