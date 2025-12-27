// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackMafia.h"

#include "AI/ACCitizenAIController.h"
#include "Character/ACCitizen.h"

UBTTask_AttackMafia::UBTTask_AttackMafia()
{
}

EBTNodeResult::Type UBTTask_AttackMafia::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AACCitizenAIController* AIController = Cast<AACCitizenAIController>(OwnerComp.GetAIOwner());
	
	// AI Controller 확인
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	AACCitizen* CitizenPawn = Cast<AACCitizen>(AIController->GetPawn());
	// AI의 유형이 시민인지 확인.
	if (CitizenPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	// 시민 공격 넣어야 함.
	CitizenPawn->MulticastPlayAttackMontage();
	
	return EBTNodeResult::Succeeded;
}
