// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ACMainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API AACMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AACMainPlayerController();

protected:
	virtual void BeginPlay() override;
};
