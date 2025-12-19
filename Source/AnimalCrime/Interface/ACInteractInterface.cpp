// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/ACInteractInterface.h"
#include "Character/ACCharacter.h"

float IACInteractInterface::GetRequiredHoldTime() const
{
	return 0.f;
}

void IACInteractInterface::ShowInteractDebug(AACCharacter* ACPlayer)
{
	if (GEngine == nullptr)
	{
		return;
	}
	if (ACPlayer == nullptr)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
		FString::Printf(TEXT("Player %s: Interacted with %s"), *ACPlayer->GetName(), *GetInteractableName())
	);
}
