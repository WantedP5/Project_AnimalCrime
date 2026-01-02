// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACDestroyableObject.h"
#include "GameFramework/Actor.h"
#include "ACBush.generated.h"

UCLASS()
class ANIMALCRIME_API AACBush : public AACDestroyableObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AACBush();

public:
	virtual void BeginPlay() override;
	virtual void PostNetInit() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> MeshComp;
};
