// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_LookAtTarget.h"

#include "AI/ACCitizenAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_LookAtTarget::UBTTask_LookAtTarget()
{
}

EBTNodeResult::Type UBTTask_LookAtTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AACCitizenAIController* AIController = Cast<AACCitizenAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	APawn* Pawn = AIController->GetPawn();
	if (Pawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
	if (BBComp == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 캐릭터 구하기.
	APawn* TargetPawn = Cast<APawn>(BBComp->GetValueAsObject(TEXT("Target")));
	if (TargetPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// NPC가 바라볼 방향 구하기.
	FVector LookVector = TargetPawn->GetActorLocation() - Pawn->GetActorLocation();

	// 회전 값 구하기.
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	// DeltaTime.
	UWorld* World = Pawn->GetWorld();



	// 회전 속도.
	float TurnSpeed = 2.0f;

	// 부드럽게 회전 처리.
	FRotator FinalRotator = FMath::RInterpTo(Pawn->GetActorRotation(), TargetRot, World->GetDeltaSeconds(), TurnSpeed);

	// 회전 설정.
	Pawn->SetActorRotation(FinalRotator);
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
