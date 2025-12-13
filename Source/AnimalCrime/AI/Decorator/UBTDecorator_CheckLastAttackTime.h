// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "UBTDecorator_CheckLastAttackTime.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UUBTDecorator_CheckLastAttackTime : public UBTDecorator_Blackboard
{
	GENERATED_BODY()
	
public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
