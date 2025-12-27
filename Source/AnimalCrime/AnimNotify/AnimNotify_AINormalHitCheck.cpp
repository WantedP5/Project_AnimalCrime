// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_AINormalHitCheck.h"

#include "Character/ACCharacter.h"
#include "Character/ACCitizen.h"

UAnimNotify_AINormalHitCheck::UAnimNotify_AINormalHitCheck()
{
}

void UAnimNotify_AINormalHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	AACCitizen* Character = Cast<AACCitizen>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Character is nullptr"));
		return ;
	}
	Character->AttackHitCheck();
}
