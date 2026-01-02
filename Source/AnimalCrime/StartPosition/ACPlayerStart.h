// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "ACPlayerStart.generated.h"

UENUM(BlueprintType)
enum class ESpawnTypeState : uint8
{
	NONE,
	POLICE,
	MAFIA,
	CITIZEN
};

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API AACPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	void PostInitializeComponents() override;
	void PostNetInit() override;
	void BeginPlay() override;
	
public:
	ESpawnTypeState GetSpawnType() const{return SpawnTypeState;}
	void SetSpawnType(ESpawnTypeState NewSpawnTypeState);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnType")
	ESpawnTypeState SpawnTypeState;
};
