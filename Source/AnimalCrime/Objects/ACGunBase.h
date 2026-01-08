// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACGunBase.generated.h"

UCLASS()
class ANIMALCRIME_API AACGunBase : public AActor
{
	GENERATED_BODY()

public:
	AACGunBase();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
