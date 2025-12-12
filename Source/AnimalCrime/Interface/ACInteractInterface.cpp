// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/ACInteractInterface.h"
#include "Character/ACCharacter.h"

void IACInteractInterface::ShowInteractDebug(AACCharacter* Interactor)
{
	if (GEngine == nullptr)
	{
		return;
	}
	if (Interactor == nullptr)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
		FString::Printf(TEXT("%s Interacted with %s"),
			*Interactor->GetName(), *GetInteractableName()));

}
