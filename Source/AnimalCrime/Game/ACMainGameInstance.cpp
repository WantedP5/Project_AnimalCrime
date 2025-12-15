// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ACMainGameInstance.h"

UACMainGameInstance::UACMainGameInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("UACMainGameInstance::UACMainGameInstance"));
	CurrentMapType = EMapType::Game;
}

EMapType UACMainGameInstance::GetMapType() const
{
	return CurrentMapType;
}

void UACMainGameInstance::UpdateMap(const EMapType InMapType)
{
	CurrentMapType = InMapType;
}

void UACMainGameInstance::UpdateLobbyMap()
{
	CurrentMapType = EMapType::Lobby;
}

void UACMainGameInstance::UpdateGameMap()
{
	CurrentMapType = EMapType::Game;
}
