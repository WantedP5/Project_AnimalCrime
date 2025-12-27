// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_UpdateTargetDistance.h"

#include "AnimalCrime.h"
#include "VectorTypes.h"
#include "AI/ACCitizenAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DataWrappers/ChaosVDQueryDataWrappers.h"

UBTService_UpdateTargetDistance::UBTService_UpdateTargetDistance()
{
}

void UBTService_UpdateTargetDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	
	AACCitizenAIController* AIController = Cast<AACCitizenAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
	{
		return;
	}
	
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (BBComp == nullptr)
	{
		return;
	}
	
	APawn* Pawn = AIController->GetPawn();
	if (Pawn == nullptr)
	{
		return;
	}
	
	APawn* TargetPawn = Cast<APawn>(BBComp->GetValueAsObject(TEXT("Target")));
	if (TargetPawn == nullptr)
	{
		return;
	}
	FVector PawnLocation = Pawn->GetActorLocation();
	FVector TargetLocation = TargetPawn->GetActorLocation();
	float Distance = FVector::Dist2D(PawnLocation, TargetLocation);
	UE_LOG(LogHY, Log, TEXT("Service Distance: %f"), Distance);
	BBComp->SetValueAsFloat(TEXT("TargetDistance"), Distance);
}
