// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_Jump.h"

#include "AnimalCrime.h"
#include "AI/ACCitizenAIController.h"
#include "Character/ACCitizen.h"

UBTTask_Jump::UBTTask_Jump()
{
}

EBTNodeResult::Type UBTTask_Jump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory); 

	AACCitizenAIController* CitizenAIController = Cast<AACCitizenAIController>(OwnerComp.GetAIOwner());
	if (CitizenAIController == nullptr)
	{
		UE_LOG(LogHY, Error, TEXT("CitizenAIController is nullptr"));
		return EBTNodeResult::Failed; 
	}
	
	AACCitizen* CitizenPawn = Cast<AACCitizen>(CitizenAIController->GetPawn());
	if (CitizenPawn == nullptr)
	{
		UE_LOG(LogHY, Error, TEXT("CitizenPawn is nullptr"));
		return EBTNodeResult::Failed;
	}
	
	CitizenPawn->JumpInPlace();
	
	return EBTNodeResult::Succeeded;
}
