// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/ACInteractInterface.h"
#include "Character/ACCharacter.h"

/**
    @brief 화면에 붉은 글씨로 플레이어와 충돌 액터를 표시.
	상호작용 확인이 필요시, OnInteraction에 호출함.
    @param ACPlayer - OnInteraction에서 받은 오버랩된 플레이어
**/
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
		FString::Printf(TEXT("Player %s: Interacted with %s"),
			*ACPlayer->GetName(), *GetInteractableName()));
}
