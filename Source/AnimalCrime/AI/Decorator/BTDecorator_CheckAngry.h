// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckAngry.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UBTDecorator_CheckAngry : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CheckAngry();
	
public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
