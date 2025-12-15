// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ACMainGameInstance.h"

// #pragma region 생성자 
// UACMainGameInstance::UACMainGameInstance()
// {
// 	UE_LOG(LogTemp, Warning, TEXT("UACMainGameInstance::UACMainGameInstance"));
// }
// #pragma endregion
//
// #pragma region 엔진 제공 함수
// void UACMainGameInstance::Init()
// {
// 	Super::Init();
// 	
// 	CurrentMapType = EMapType::Game;
// 	
// 	LobbyMapName = TEXT("LobbyMap");
// 	GameMapName = TEXT("HenaMap");
// 	
// 	UE_LOG(LogTemp, Warning, TEXT("UACMainGameInstance::Init"));
// }
//
// void UACMainGameInstance::Shutdown()
// {
// 	Super::Shutdown();
// 	
// 	UE_LOG(LogTemp, Warning, TEXT("UACMainGameInstance::Shutdown"));
// }
//
// void UACMainGameInstance::OnStart()
// {
// 	Super::OnStart();
// 	
// 	UE_LOG(LogTemp, Warning, TEXT("UACMainGameInstance::OnStart"));
// }
// #pragma endregion
//
// #pragma region Map Level
// void UACMainGameInstance::LoadLobbyMap()
// {
// 	UpdateMap(EMapType::Lobby);
// }
//
// void UACMainGameInstance::LoadGameMap()
// {
// 	UpdateMap(EMapType::Game);
// }
//
// EMapType UACMainGameInstance::GetMapType() const
// {
// 	return CurrentMapType;
// }
//
// void UACMainGameInstance::UpdateMap(const EMapType InMapType)
// {
// 	if (CurrentMapType == InMapType)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("CurrentMapType == InMapType"));
// 		return ;
// 	}
//
// 	switch (InMapType)
// 	{
// 	case EMapType::Lobby:
// 		GetWorld()->ServerTravel(LobbyMapName);
// 		break;
// 	case EMapType::Game:
// 		GetWorld()->ServerTravel(GameMapName);
// 		break;
// 	default:
// 		break;
// 	}
// 	
// 	// 현재 맵을 변경된 맵으로 Update
// 	CurrentMapType = InMapType;
// }
// #pragma endregion
