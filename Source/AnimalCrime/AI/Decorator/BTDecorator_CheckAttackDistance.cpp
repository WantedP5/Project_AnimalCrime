// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_CheckAttackDistance.h"

#include "AnimalCrime.h"
#include "AI/ACCitizenAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/ACCitizen.h"

UBTDecorator_CheckAttackDistance::UBTDecorator_CheckAttackDistance()
{
}

bool UBTDecorator_CheckAttackDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	AACCitizenAIController* AIController =  Cast<AACCitizenAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
	{
		UE_LOG(LogHY, Error, TEXT("AIController is nullptr"));	
		return false;
	}
	
	AACCitizen* CitizenPawn = Cast<AACCitizen>(AIController->GetPawn());
	if (CitizenPawn == nullptr)
	{
		UE_LOG(LogHY, Error, TEXT("CitizenPawn is nullptr"));
		return false;
	}
	
	UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
	if (BBComp == nullptr)
	{
		UE_LOG(LogHY, Error, TEXT("BBComp is nullptr"));
		return false;
	}
	
	APawn* TargetPawn = Cast<APawn>(BBComp->GetValueAsObject(TEXT("Target")));
	if (TargetPawn == nullptr)
	{
		UE_LOG(LogHY, Error, TEXT("TargetPawn is nullptr"));
		return false;
	}
	
	FVector Location = CitizenPawn->GetActorLocation();
	FVector TargetLocation = TargetPawn->GetActorLocation();
	
	float Distance = BBComp->GetValueAsFloat(TEXT("TargetDistance")); 
		//FVector::Distance(Location, TargetLocation);
	
	UE_LOG(LogHY, Error, TEXT("Dist %f %f"), Distance, Distance*Distance);
	
	return 40000 >= Distance * Distance;
}
