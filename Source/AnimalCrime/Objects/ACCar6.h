// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACDestroyableObject.h"
#include "ACCar6.generated.h"

UCLASS()
class ANIMALCRIME_API AACCar6 : public AACDestroyableObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AACCar6();

public:
	virtual void BeginPlay() override;
	virtual void PostNetInit() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
};
