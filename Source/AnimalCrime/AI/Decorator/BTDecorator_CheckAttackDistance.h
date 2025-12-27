// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckAttackDistance.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UBTDecorator_CheckAttackDistance : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CheckAttackDistance();
	
public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
