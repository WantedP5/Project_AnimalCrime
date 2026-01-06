// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ACCitizenData.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACCitizenData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DebugDelta = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DebugDrawLifeTime = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RegenRateMin = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RegenRateMax = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropMoneyMin;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropMoneyMax;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropTime = 1.0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCapsuleRadius = 30.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCapsuleHalfHeight = 60.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackTraceDistance = 200.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDamage = 30.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectRadius = 500.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultWalkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunWalkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AngryWalkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MafiaScore;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PoliceScore;
};
