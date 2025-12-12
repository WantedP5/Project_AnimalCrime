// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Objects/DestroyableStats.h"

#include "ACDestroyableStatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ANIMALCRIME_API UACDestroyableStatComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region 특수 맴버 함수
public:
	
	UACDestroyableStatComponent();
#pragma endregion


#pragma region 엔진 제공 함수
public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

#pragma region 스탯 관련 함수
public:
	UFUNCTION(BlueprintCallable)
	void OnDamaged(float DamageAmount);
	
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxHp() const{return DestroyableStats.MaxHp; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentHp() const{return DestroyableStats.CurrentHp; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetArmor() const{return DestroyableStats.Armor; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetScorePoint() const{return DestroyableStats.ScorePoint; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetMaxHp(float NewMaxHp){DestroyableStats.MaxHp = NewMaxHp; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCurrentHp(float NewCurrentHp){ DestroyableStats.CurrentHp = NewCurrentHp; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetArmor(float NewArmor) { DestroyableStats.Armor = NewArmor; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetScorePoint(float NewScorePoint){ DestroyableStats.ScorePoint = NewScorePoint; }
#pragma endregion
	
#pragma region 스탯
private:
	UPROPERTY(EditAnywhere)
	FDestroyableStats DestroyableStats;
#pragma endregion
};
