// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_AttackMafiaByTick.h"

#include "AnimalCrime.h"
#include "AI/ACCitizenAIController.h"
#include "Character/ACCitizen.h"

UBTTask_AttackMafiaByTick::UBTTask_AttackMafiaByTick()
{
	bNotifyTick = 1;
}

EBTNodeResult::Type UBTTask_AttackMafiaByTick::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogHY, Warning, TEXT("ExecuteTask Begin"));
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	
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
	
	CitizenPawn->MulticastPlayAttackMontage();
	
	UE_LOG(LogHY, Warning, TEXT("ExecuteTask End"));
	
	return EBTNodeResult::InProgress;
}

void UBTTask_AttackMafiaByTick::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// UE_LOG(LogHY, Warning, TEXT("TickTask Begin"));
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	AACCitizenAIController* CitizenAIController = Cast<AACCitizenAIController>(OwnerComp.GetAIOwner());
	if (CitizenAIController == nullptr)
	{
		UE_LOG(LogHY, Error, TEXT("CitizenAIController is nullptr"));
		return; 
	}
	
	AACCitizen* CitizenPawn = Cast<AACCitizen>(CitizenAIController->GetPawn());
	if (CitizenPawn == nullptr)
	{
		UE_LOG(LogHY, Error, TEXT("CitizenPawn is nullptr"));
		return;
	}
	
	UAnimInstance* AnimInstance = CitizenPawn->GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}
	
	if (AnimInstance->Montage_IsPlaying(CitizenPawn->MeleeMontage) == true)
	{
		return;
	}
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	
	//UE_LOG(LogHY, Warning, TEXT("TickTask End"));
}
