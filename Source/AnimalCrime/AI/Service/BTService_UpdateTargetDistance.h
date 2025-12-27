// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateTargetDistance.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UBTService_UpdateTargetDistance : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_UpdateTargetDistance();
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
