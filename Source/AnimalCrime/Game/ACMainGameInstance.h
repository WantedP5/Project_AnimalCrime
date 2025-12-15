// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ACMainGameInstance.generated.h"

UENUM(BlueprintType)
enum class EMapType : uint8
{
	None		UMETA(DisplayName="None"),
	Lobby       UMETA(DisplayName="Lobby"),
	Game        UMETA(DisplayName="Game"),
	Result      UMETA(DisplayName="Result"),
	Shop	    UMETA(DisplayName="Shop"),
	Unknown     UMETA(DisplayName="Unknown")
};

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UACMainGameInstance();
	
public:
	EMapType GetMapType() const;
	void UpdateMap(EMapType InMapType);
	void UpdateLobbyMap();
	void UpdateGameMap();
	
private:
	EMapType CurrentMapType;
};
