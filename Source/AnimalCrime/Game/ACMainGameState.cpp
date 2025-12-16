// Fill out your copyright notice in the Description page of Project Settings.


#include "ACMainGameState.h"

#include "AnimalCrime.h"
#include "Net/UnrealNetwork.h"

#include "Character/ACMafiaCharacter.h"

void AACMainGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AACMainGameState, TeamScore);
	DOREPLIFETIME(AACMainGameState, EscapeState);
}

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
		return nullptr;
	}
	
	// @InComplete
	return DestinationObjects[0];
}

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
