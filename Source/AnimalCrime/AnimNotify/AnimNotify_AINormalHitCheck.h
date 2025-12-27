// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AINormalHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UAnimNotify_AINormalHitCheck : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_AINormalHitCheck();
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
