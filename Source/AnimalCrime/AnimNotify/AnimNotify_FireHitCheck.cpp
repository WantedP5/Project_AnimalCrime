// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_FireHitCheck.h"

#include "AnimalCrime.h"
#include "Character/ACCharacter.h"

UAnimNotify_FireHitCheck::UAnimNotify_FireHitCheck()
{
}

void UAnimNotify_FireHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	AACCharacter* Character = Cast<AACCharacter>(MeshComp->GetOwner());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Character is nullptr"));
		return ;
	}
	UE_LOG(LogHY, Log, TEXT("Character is %s"), *Character->GetName());
	
	if (!Character->IsLocallyControlled())
	{
		return;
	}
	// 서버에 요청
	Character->FireHitscan();
	//Character->AttackHitCheck();
}
