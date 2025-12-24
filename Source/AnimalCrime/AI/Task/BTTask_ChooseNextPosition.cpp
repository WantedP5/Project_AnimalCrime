// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChooseNextPosition.h"

#include "AIController.h"
#include "AI/ACCitizenAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/ACCitizen.h"

UBTTask_ChooseNextPosition::UBTTask_ChooseNextPosition()
{
}

EBTNodeResult::Type UBTTask_ChooseNextPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AACCitizenAIController* AIController = Cast<AACCitizenAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	AACCitizen* NPCPawn = Cast<AACCitizen>(AIController->GetPawn());
	if (NPCPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AIController is nullptr"));
		return EBTNodeResult::Failed;
	}
	
	UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
	if (BBComp == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("BBComp is nullptr"));
		return EBTNodeResult::Failed;
	}
	
	FVector NextPosition = NPCPawn->GetNextPosition();
	BBComp->SetValueAsVector(TEXT("Position"), NextPosition);
	//UE_LOG(LogTemp, Log, TEXT("Next Position: (%f, %f, %f)"), NextPosition.X, NextPosition.Y, NextPosition.Z);
	
	return EBTNodeResult::Succeeded;
}
