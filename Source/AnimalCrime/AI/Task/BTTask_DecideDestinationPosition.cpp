// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_DecideDestinationPosition.h"

#include "NavigationSystem.h"
#include "AI/ACCitizenAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/ACMainGameState.h"

UBTTask_DecideDestinationPosition::UBTTask_DecideDestinationPosition()
{
	
}

EBTNodeResult::Type UBTTask_DecideDestinationPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AACMainGameState* MainGameState = Cast<AACMainGameState>(GetWorld()->GetGameState());
	if (MainGameState == nullptr)
	{
		return EBTNodeResult::Type::Failed;
	}

	AACCitizenAIController* AIController = Cast<AACCitizenAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
	{
		return EBTNodeResult::Type::Failed;
	}
	
	AActor* DestinationActor = MainGameState->GetDestinationActor();
	if (DestinationActor == nullptr)
	{
		return EBTNodeResult::Type::Failed;
	}
	
	UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
	if (BBComp == nullptr)
	{
		return EBTNodeResult::Type::Failed;
	}
	
	
	FVector DestinationPosition = DestinationActor->GetActorLocation();
	FVector TargetLocation = FVector::Zero();
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation Projected;
	const FVector Extent(200.f, 200.f, 300.f);
	if (NavSys->ProjectPointToNavigation(DestinationPosition, Projected, Extent))
	{
		TargetLocation = Projected.Location;
	}

	BBComp->SetValueAsVector(TEXT("DestinationPosition"), TargetLocation);
	
	
	return EBTNodeResult::Type::Succeeded; 
}
