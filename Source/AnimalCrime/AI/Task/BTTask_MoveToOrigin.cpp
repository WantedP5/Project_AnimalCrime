// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_MoveToOrigin.h"

#include "AIController.h"
#include "Character/ACCitizen.h"

UBTTask_MoveToOrigin::UBTTask_MoveToOrigin()
{
}

EBTNodeResult::Type UBTTask_MoveToOrigin::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	
	return SuperResult;
}

void UBTTask_MoveToOrigin::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	
	UE_LOG(LogTemp, Log, TEXT("Task Move To Origin Finished"));
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AIController is nullptr"));
		return;
	}
	
	AACCitizen* CitizenPawn = Cast<AACCitizen>(AIController->GetPawn());
	if (CitizenPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("CitizenPawn is nullptr"));
		return;
	}
	
	if (bAbortFlag == false)
	{
		CitizenPawn->OnArrive();
	}
}

EBTNodeResult::Type UBTTask_MoveToOrigin::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Log, TEXT("AbortTask Move To Origin"));
	bAbortFlag = true;
	return Super::AbortTask(OwnerComp, NodeMemory);
}
