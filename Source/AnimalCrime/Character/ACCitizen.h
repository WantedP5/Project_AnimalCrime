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
	virtual void PostInitializeComponents() override;
	virtual void PostNetInit() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void PlayDamagedMontage(const FVector& Attack);
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
#pragma endregion
	
public:	// 좌표
	FVector GetNextPosition() const;
	
#pragma region 인터랙션 인터페이스
	//!< 상호작용 인터페이스
protected:
	virtual bool CanInteract(class AACCharacter* ACPlayer) override;
	virtual void OnInteract(class AACCharacter* ACPlayer) override;
	virtual float GetRequiredHoldTime() const override;
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
	
	void AttackHitCheck();
	

#pragma region 매시 Get/Set
	FORCEINLINE USkeletalMesh* GetHeadMesh()		const { return HeadMesh;}
	FORCEINLINE USkeletalMesh* GetFaceMesh()		const {	return FaceMesh;}
	FORCEINLINE USkeletalMesh* GetTopMesh()			const {	return TopMesh;}
	FORCEINLINE USkeletalMesh* GetBottomMesh()		const {	return BottomMesh;}
	FORCEINLINE USkeletalMesh* GetShoesMesh()		const {	return ShoesMesh;}
	FORCEINLINE USkeletalMesh* GetFaceAccMesh()		const {	return FaceAccMesh;}
	
	FORCEINLINE void UpdateHeadMesh()		const {	HeadMeshComp->SetSkeletalMesh(HeadMesh);	}
	FORCEINLINE void UpdateFaceMesh()		const {	FaceMeshComp->SetSkeletalMesh(FaceMesh);	}
	FORCEINLINE void UpdateTopMesh()		const {	TopMeshComp->SetSkeletalMesh(TopMesh);	}
	FORCEINLINE void UpdateBottomMesh()		const {	BottomMeshComp->SetSkeletalMesh(BottomMesh);	}
	FORCEINLINE void UpdateShoesMesh()		const {	ShoesMeshComp->SetSkeletalMesh(ShoesMesh);	}
	FORCEINLINE void UpdateFaceAccMesh()	const {	FaceAccMeshComp->SetSkeletalMesh(FaceAccMesh);	}
	
	FORCEINLINE void SetHeadMesh(USkeletalMesh* InMesh)			 {  HeadMesh = InMesh;}
	FORCEINLINE void SetFaceMesh(USkeletalMesh* InMesh)			 {	FaceMesh = InMesh;}
	FORCEINLINE void SetTopMesh(USkeletalMesh* InMesh)			 {	TopMesh = InMesh;}
	FORCEINLINE void SetBottomMesh(USkeletalMesh* InMesh)		 {	BottomMesh = InMesh;}
	FORCEINLINE void SetShoesMesh(USkeletalMesh* InMesh)		 {	ShoesMesh = InMesh;}
	FORCEINLINE void SetFaceAccMesh(USkeletalMesh* InMesh)		 {	FaceAccMesh = InMesh;}
#pragma endregion
	
#pragma region 매시 변경 시 호출되는 함수
public:
	UFUNCTION()
	void OnRep_HeadMesh() const;

	UFUNCTION()
	void OnRep_FaceMesh() const;
	
	UFUNCTION()
	void OnRep_TopMesh() const;

	UFUNCTION()
	void OnRep_BottomMesh() const;
	
	UFUNCTION()
	void OnRep_ShoesMesh() const;

	UFUNCTION()
	void OnRep_FaceAccMesh() const;
#pragma endregion 
	
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
	
	
	
	

	
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<class UACMoneyComponent> MoneyComp;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<class USkeletalMeshComponent> HeadMeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<class USkeletalMeshComponent> FaceMeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<class USkeletalMeshComponent> TopMeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<class USkeletalMeshComponent> BottomMeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<class USkeletalMeshComponent> ShoesMeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<class USkeletalMeshComponent> FaceAccMeshComp;
	
	UPROPERTY(ReplicatedUsing=OnRep_HeadMesh)
	TObjectPtr<class USkeletalMesh> HeadMesh;
	
	UPROPERTY(ReplicatedUsing=OnRep_FaceMesh)
	TObjectPtr<class USkeletalMesh> FaceMesh;
	
	UPROPERTY(ReplicatedUsing=OnRep_TopMesh)
	TObjectPtr<class USkeletalMesh> TopMesh;
	
	UPROPERTY(ReplicatedUsing=OnRep_BottomMesh)
	TObjectPtr<class USkeletalMesh> BottomMesh;
	
	UPROPERTY(ReplicatedUsing=OnRep_ShoesMesh)
	TObjectPtr<class USkeletalMesh> ShoesMesh;
	
	UPROPERTY(ReplicatedUsing=OnRep_FaceAccMesh)
	TObjectPtr<class USkeletalMesh> FaceAccMesh;
	
private:
#pragma endregion
};
