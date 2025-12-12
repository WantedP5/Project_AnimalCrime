// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_NormalHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UAnimNotify_NormalHitCheck : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_NormalHitCheck();
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
