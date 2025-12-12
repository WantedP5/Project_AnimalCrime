// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RunAwayOnDamage.h"

#include "AIController.h"
#include "Character/ACCitizen.h"

UBTTask_RunAwayOnDamage::UBTTask_RunAwayOnDamage()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_RunAwayOnDamage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult =  Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	AActor* AIOwner = AIController->GetOwner();
	AActor* OwnerCompOwner = OwnerComp.GetOwner();

	ensure(AIOwner == OwnerCompOwner);
	
	AACCitizen* CitizenPawn = Cast<AACCitizen>(AIOwner);
	if (CitizenPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_RunAwayOnDamage::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_RunAwayOnDamage::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
