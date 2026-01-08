// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ACAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	// GameThread
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Worker Thread (Thread Safe)
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(Transient)
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;
	
	/* ================= GameThread 데이터 ================= */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Movement")
	bool bIsFalling = false;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Character")
	TObjectPtr<class ACharacter> Character;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Character")
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Movement")
	float GroundSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Movement")
	bool bShouldMove = false;

	/* ================= Thread Safe 계산 결과 ================= */

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Movement")
	float Direction = 0.f;
};
