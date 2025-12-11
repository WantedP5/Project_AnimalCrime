// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ACCharacter.h"
#include "ACTestPoliceCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API AACTestPoliceCharacter : public AACCharacter
{
	GENERATED_BODY()
	
public:
	AACTestPoliceCharacter();

	virtual EACCharacterType GetCharacterType() override;

protected:
	//!< 메쉬 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;



public:
protected:
	virtual bool CanInteract(AACCharacter* Interactor) override;		// 누가 상호작용 가능한지(캐릭터 타입 체크) |
	virtual void OnInteract(AACCharacter* Interactor) override;		// 실제 상호작용 로직(서버에서 실행) |
};
