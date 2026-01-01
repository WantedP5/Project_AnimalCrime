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

public:
	virtual void PostInitializeComponents() override;
	virtual void PostNetInit() override;
	virtual void BeginPlay() override;

public:
	virtual EACCharacterType GetCharacterType() override;

	virtual void AttackHitCheck() override;
	
	
public:
	/**
	 * @brief TimeRate마다 월급을 주는 함수
	 */
	void CalculateSalary();

	/**
	 * @brief TimeRate와 기존 Timer 제거 후 재실행
	 * @param InTimeRate 
	 */
	void ChangeSalary(float InTimeRate);

public:
protected:
	virtual bool CanInteract(AACCharacter* ACPlayer) override;
	virtual void OnInteract(AACCharacter* ACPlayer) override;

protected:
	// 월급
	FTimerHandle SalaryTimerHandle;
	float TimeRate = 10.0f;
	
};
