// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACBullet.generated.h"

UCLASS()
class ANIMALCRIME_API AACBullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AACBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
