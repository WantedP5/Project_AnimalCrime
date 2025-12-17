// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ACCharacter.h"
#include "ACPoliceCharacter.generated.h"

/**
 *
 */
UCLASS()
class ANIMALCRIME_API AACPoliceCharacter : public AACCharacter
{
	GENERATED_BODY()

public:
	AACPoliceCharacter();

	virtual EACCharacterType GetCharacterType() override;

public:
	//!< 상호작용 인터페이스
protected:
	virtual bool CanInteract(AACCharacter* ACPlayer) override;
	virtual void OnInteract(AACCharacter* ACPlayer) override;
	virtual FString GetInteractableName() const override;



protected:
	//!< 메쉬 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;
};
