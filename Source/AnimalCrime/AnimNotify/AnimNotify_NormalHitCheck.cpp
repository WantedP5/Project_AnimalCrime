// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_NormalHitCheck.h"

#include "Character/ACCharacter.h"

UAnimNotify_NormalHitCheck::UAnimNotify_NormalHitCheck()
{
}

void UAnimNotify_NormalHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	AACCharacter* Character = Cast<AACCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Character is nullptr"));
		return ;
	}
	Character->AttackHitCheck();
}
