// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DecideNextCooldown.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_DecideNextCooldown::UBTTask_DecideNextCooldown()
{
}

EBTNodeResult::Type UBTTask_DecideNextCooldown::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		return EBTNodeResult::Type::Failed;
	}
	
	UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
	if (BBComp == nullptr)
	{
		return EBTNodeResult::Type::Failed;
	}
	
	float NextCooldown = FMath::RandRange(1, 10);
	BBComp->SetValueAsFloat("NextCooldown", NextCooldown);
	
	return EBTNodeResult::Type::Succeeded;
}
