// Fill out your copyright notice in the Description page of Project Settings.


#include "UBTDecorator_CheckLastAttackTime.h"

#include "AIController.h"
#include "Character/ACCitizen.h"

bool UUBTDecorator_CheckLastAttackTime::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool SuperResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	

	if (SuperResult == false)
	{
		return SuperResult;
	}
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		return false;
	}
	AACCitizen* CitizenPawn = Cast<AACCitizen>(AIController->GetPawn());
	if (CitizenPawn == nullptr)
	{
		return false;
	}
	
	//FloatValue = CitizenPawn->GetLastHitTime();
	return SuperResult;
}
