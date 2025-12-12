// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ChooseNextPosition.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UBTTask_ChooseNextPosition : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ChooseNextPosition();
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
