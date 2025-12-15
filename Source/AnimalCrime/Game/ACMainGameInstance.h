// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ACMainGameInstance.generated.h"

// UENUM(BlueprintType)
// enum class EMapType : uint8
// {
// 	None		UMETA(DisplayName="None"),
// 	Lobby       UMETA(DisplayName="Lobby"),
// 	Game        UMETA(DisplayName="Game"),
// 	Result      UMETA(DisplayName="Result"),
// 	Shop	    UMETA(DisplayName="Shop"),
// 	Unknown     UMETA(DisplayName="Unknown")
// };

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
// #pragma region 생성자 
// public:
// 	UACMainGameInstance();
// #pragma endregion
// 	
// #pragma region 엔진 제공 함수
// public:
// 	virtual void Init() override;
// 	virtual void Shutdown() override;
// 	virtual void OnStart() override;
// #pragma endregion
//
// #pragma region Map Level 
// public:
// 	void LoadLobbyMap();
// 	void LoadGameMap();
//
// 	EMapType GetMapType() const;
// 	
// private:
// 	void UpdateMap(EMapType InMapType);
// #pragma endregion

#pragma region Map Level 관련 맴버 변수 
private:
// 	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
// 	EMapType CurrentMapType;
// 	
// 	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
// 	FString LobbyMapName;
// 	
// 	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
// 	FString GameMapName;
// #pragma endregion
};
