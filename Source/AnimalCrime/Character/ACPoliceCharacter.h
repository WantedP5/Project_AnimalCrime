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
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual EACCharacterType GetCharacterType() const override;

	virtual void AttackHitCheck() override;
	
	void UpdateCharacterStatusRevive();
	
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

protected:
	virtual void OnInteract(AACCharacter* ACPlayer, EInteractionKey InKey) override;
	virtual EACInteractorType GetInteractorType() const override;

protected:
	// 월급
	FTimerHandle SalaryTimerHandle;
	float TimeRate = 10.0f;
	
};
