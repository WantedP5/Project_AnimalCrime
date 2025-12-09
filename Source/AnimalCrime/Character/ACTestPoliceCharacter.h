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

protected:
	//!< 메쉬 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;
};
