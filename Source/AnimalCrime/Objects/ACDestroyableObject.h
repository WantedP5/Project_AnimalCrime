// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACDestroyableObject.generated.h"

UCLASS()
class ANIMALCRIME_API AACDestroyableObject : public AActor
{
	GENERATED_BODY()

public:
	AACDestroyableObject();

protected:
	virtual void BeginPlay() override;
	virtual void PostNetInit() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateEarnScore(float InScore) {EarnScore = InScore;}
	
	UFUNCTION(BlueprintCallable)
	float GetCurrentEarnScore() const {return EarnScore;}
	
protected:
	void HandleDestroyed(AController* InstigatorController);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USceneComponent> RootComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UACDestroyableStatComponent> DestroyComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EarnScore;
};
