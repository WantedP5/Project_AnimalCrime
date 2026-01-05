// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ACInteractInterface.h"
#include "Game/ACGameEnums.h"
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
	/**
	 * @brief  게임과 관련없는 액터의 속성 초기화 함수
	 */
	virtual void PostInitializeComponents() override;
	
	/**
	* @brief  네트워크로 복제된 액터가 클라이언트에 처음 완전히 도착했을 때 호출되는 함수
	 */
	virtual void PostNetInit() override;
	
	/**
	 * @brief  게임과 관련있는 액터의 속성 초기화 함수
	 */
	virtual void BeginPlay() override;
	
	/**
	 * @brief  틱마다 호출되는 함수
	 * @param DeltaTime 
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief  네트워크로 복제할 속성를 등록하는 함수
	 * @param OutLifetimeProps 
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * @brief  데미지를 입었을 때 호출되는 함수
	 * @param DamageAmount 
	 * @param DamageEvent 
	 * @param EventInstigator 
	 * @param DamageCauser 
	 * @return 
	 */
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
#pragma endregion

#pragma region 인터랙션 인터페이스
	//!< 상호작용 인터페이스
protected:
	virtual void OnInteract(class AACCharacter* ACPlayer, EInteractionKey InKey) override;
	virtual EACInteractorType GetInteractorType() const override;

	//=== NEW: Widget interface overrides ===
	virtual class UWidgetComponent* GetInteractionWidget() const override;
	virtual void ShowInteractionHints(const TArray<class UACInteractionData*>& Interactions) override;
	virtual void HideInteractionHints() override;
#pragma endregion

public: // 임시
	FVector GetNextPosition() const;
	void PlayDamagedMontage(const FVector& Attack);
	FVector GetRunPosition(const FVector& Attack) const;

	UFUNCTION(BlueprintCallable)
	int32 GetDamageFlag() { return DamagedFlag; }
	float GetLastHitTime() const;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnPlayMontage(const FVector& Attack);

	void AttackHitCheck();
	
public:
	void UpdateAISkillFlag();
	void UpdateCharacterStatusFree();
	void UpdateCharacterStatusRevive();
	
#pragma region 상태 변화 함수
public:
	/**
	 * @brief  목적지 도착시 호출되는 함수
	 */
	void OnArrive();
	
	/**
	 * @brief  최종 상태 변화 함수
	 */
	void OnChangeState();
	
	/**
	 * @brief  피격 시 호출되는 함수
	 */
	void OnDamaged();
	
	/**
	 * @brief  피격 시 점수 변환 함수
	 * @param Actor 피해를 준 액터 객체 
	 */
	void OnUpdateScore(AActor* Actor);
	/**
	 * @brief  피격 시 돈 변경 함수
	 * @param Actor 피해를 준 액터 객체 
	 */
	void OnUpdateMoney(AActor* Actor);
#pragma endregion

#pragma region AI 스킬
public:
	/**
	 * @brief  AI의 옷 변경 스킬 함수
	 */
	void ChangeClothes();
	
	/**
	 * @brief  경찰 감지 함수
	 */
	bool DetectPolice();
	
	/**
	 * @brief  경찰로 부터 도망가는 스킬 함수
	 */
	void RunFromPolice();
	
	/**
	 * @brief  경찰로 부터 도망가는 스킬 함수
	 */
	void JumpInPlace();
	
#pragma endregion

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

	//!< 상호작용 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact")
	TObjectPtr<class UWidgetComponent> InteractionWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TSubclassOf<class UACInteractionInfoWidget> InteractionInfoWidgetClass;

public:
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayAttackMontage();
	
	UPROPERTY(meta=(AllowPrivateAccess=true))
	int32 DamagedFlag;

	UPROPERTY(meta=(AllowPrivateAccess=true))
	float LastHitTime;

	FTimerHandle InitialSkillBlockTimerHandle;

	FORCEINLINE bool IsInitialSkillBlocked() const { return bIsInitialSkillBlocked; }
	bool bIsInitialSkillBlocked = true;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UAnimMontage> DamagedMontage;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<class UACMoneyComponent> MoneyComp;
	
	// 돈 쿨타임 
	FTimerHandle MoneyCoolTimerHandle;
	bool bMoneyCoolTimerActivated = false;
	
	// 속도 쿨타임 
	FTimerHandle MoveSpeedTimerHandle;
	
	UPROPERTY(ReplicatedUsing = OnRep_CharacterState)
	ECharacterState CharacterState = ECharacterState::None;
	
	ECharacterState TempCharacterState = ECharacterState::None;

	
	/** 몽타주: 기본 공격  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member|Attack|Anim", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MeleeMontage;
	
	UFUNCTION()
	void OnRep_CharacterState();

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

	UPROPERTY()
	TObjectPtr<class AACPoliceCharacter> PoliceCharacter;
	
	UPROPERTY()
	TObjectPtr<class AACMafiaCharacter> MafiaCharacter;
private:
	float DebugDelta = 1.0f;
	
public:
	void RegenMoney();
	void TryRegenMoneyTimer();
	
protected:
	FTimerHandle RegenMoneyTimerHandle;
	float RegenRateMin = 20.0f;
	float RegenRateMax = 60.0f;
#pragma endregion
};
