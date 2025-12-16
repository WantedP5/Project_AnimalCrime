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

protected:
	//!< 메쉬 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;



public:
protected:
	virtual bool CanInteract(AACCharacter* ACPlayer) override;		// 누가 상호작용 가능한지(캐릭터 타입 체크) |
	virtual void OnInteract(AACCharacter* ACPlayer) override;		// 실제 상호작용 로직(서버에서 실행) |
	virtual FString GetInteractableName() const override;
};
