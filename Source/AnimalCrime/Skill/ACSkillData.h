// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ACSkillData.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACSkillData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SprintMoveSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float OriginMafiaMoveSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float OriginPoliceMoveSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SprintGauge;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DashForwardImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DashUpwardImpulse;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DashCoolTime;
};
