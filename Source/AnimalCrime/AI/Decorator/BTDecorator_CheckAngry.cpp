// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_CheckAngry.h"

#include "AnimalCrime.h"
#include "AI/ACCitizenAIController.h"
#include "Character/ACCitizen.h"

class AACCitizen;
class AACCitizenAIController;

UBTDecorator_CheckAngry::UBTDecorator_CheckAngry()
{
}

bool UBTDecorator_CheckAngry::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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
	ECharacterState State = CitizenPawn->CharacterState;
	
	UE_LOG(LogHY, Error, TEXT("AI State: %s"),*StaticEnum<ECharacterState>()->GetValueAsString(State));
	
	return State == ECharacterState::Angry;
}
