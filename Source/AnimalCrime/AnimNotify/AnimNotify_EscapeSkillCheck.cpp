// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_EscapeSkillCheck.h"

#include "Character/ACCharacter.h"
#include "Character/ACMafiaCharacter.h"

UAnimNotify_EscapeSkillCheck::UAnimNotify_EscapeSkillCheck()
{
}

void UAnimNotify_EscapeSkillCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	AACMafiaCharacter* MafiaPawn = Cast<AACMafiaCharacter>(MeshComp->GetOwner());
	if (MafiaPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("MafiaPawn is nullptr"));
		return ;
	}
	MafiaPawn->AttackHitCheck(5);
}
