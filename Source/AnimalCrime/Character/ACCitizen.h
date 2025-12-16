// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ACInteractInterface.h"
#include "ACCitizen.generated.h"

UCLASS()
class ANIMALCRIME_API AACCitizen : public ACharacter, public IACInteractInterface
{
	GENERATED_BODY()

	enum class State
	{
		Front,
		Back,
		Left,
		Right
	};
	
#pragma region 특수 맴버 함수
public:
	AACCitizen();
#pragma endregion
	
#pragma region 엔진 제공 함수
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void PlayDamagedMontage(const FVector& Attack);
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
#pragma endregion
	
public:	// 좌표
	FVector GetNextPosition() const;
	
#pragma region 인터랙션 인터페이스
	//!< 상호작용 인터페이스
protected:
	virtual bool CanInteract(class AACCharacter* ACPlayer) override;
	virtual void OnInteract(class AACCharacter* ACPlayer) override;
	virtual FString GetInteractableName() const override;
#pragma endregion

public:
	FVector GetRunPosition(const FVector& Attack) const;
	
	UFUNCTION(BlueprintCallable)
	int32 GetDamageFlag() {return DamagedFlag;}
	
	void OnDamaged();
	
	void OnArrive();
	
	float GetLastHitTime() const;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnPlayMontage(const FVector& Attack);
#pragma region 맴버 변수
public:

protected:
	//!< 상호작용 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TObjectPtr<class UACInteractableComponent> InteractBoxComponent;
	
public:
	UPROPERTY(meta=(AllowPrivateAccess=true))
	int32 DamagedFlag;
	
	UPROPERTY(meta=(AllowPrivateAccess=true))
	float LastHitTime;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UAnimMontage> DamagedMontage;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> HeadMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> TopMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> BottomMesh;

private:
#pragma endregion
};
