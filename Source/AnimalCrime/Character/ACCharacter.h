#pragma once

#include "CoreMinimal.h"
#include "Game/ACGameEnums.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/SkeletalMeshComponent.h"
#include "Interface/ACInteractInterface.h"
#include "ACCharacter.generated.h"

UCLASS()
class ANIMALCRIME_API AACCharacter : public ACharacter, public IACInteractInterface
{
	GENERATED_BODY()

public:
	AACCharacter();
 /**
     @brief  캐릭터 정보를 반환하는 함수. 캐릭터 베이스는 시민.
     @retval  - 캐릭터 정보 Enum
 **/
	virtual EACCharacterType GetCharacterType();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	/**
		@brief 현재 입력매핑 컨텍스트를 전부 지우고 새로운 입력매핑 컨텍스트로 바꾸는 함수
		@param NewMode - 입력모드 Enum
	**/
	void ChangeInputMode(EInputMode NewMode);

	// ===== 입력 핸들러 (PlayerController가 호출) =====
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	virtual void InteractStarted();
	virtual void InteractHolding(const float DeltaTime);
	virtual void InteractReleased();

	virtual void ItemDrop();
	virtual void Attack();
	//virtual void SettingsClose(const FInputActionValue& Value);
	virtual void Jump() override;
	virtual void SettingsClose();
	

	// 클라이언트 Input을 받는 함수
	void Dash(const FInputActionValue& Value);

	// 서버에 Dash 요청 함수
	UFUNCTION(Server, Reliable)
	void ServerDash();
	
	// 쿨타임 시 Flag 되돌리는 함수.
	void ResetDashFlag();
	
	
	// 클라이언트 Input을 받는 함수
	void Sprint(const FInputActionValue& Value);

	// 서버에 Dash 요청 함수
	UFUNCTION(Server, Reliable)
	void ServerSprintStart();
	
	// 서버에 Dash 요청 함수
	UFUNCTION(Server, Reliable)
	void ServerSprintEnd();
	
	// 쿨타임 시 Flag 되돌리는 함수.
	void ResetSprint();
	
protected:

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* Target);

	UFUNCTION(Server, Reliable)
	virtual void ServerItemDrop();

	UFUNCTION(Server, Reliable)
	void ServerSetTargetState(AACCharacter* Target, ECharacterState NewState);

	UFUNCTION(Server, Reliable)
	void ServerFreezeCharacter(ACharacter* Target, bool bFreeze);

public:

	UFUNCTION(BlueprintCallable)
	bool CheckProcessAttack() const;
	UFUNCTION(BlueprintCallable)
	void ChangeAttackTrue();
	UFUNCTION(BlueprintCallable)
	void ChangeAttackFalse();
	

public:
	// TObjectPtr<class USkeletalMeshComponent> GetHeadMesh() const { return HeadMesh; }
	// TObjectPtr<class USkeletalMeshComponent> GetFaceAccMesh() const { return FaceAccMesh; }
	// TObjectPtr<class USkeletalMeshComponent> GetTopMesh() const { return TopMesh; }
	// TObjectPtr<class USkeletalMeshComponent> GetBottomMesh() const { return BottomMesh; }
	// TObjectPtr<class USkeletalMeshComponent> GetShoesMesh() const { return ShoesMesh; }
	TObjectPtr<class USkeletalMeshComponent> GetHeadMesh() const { return HeadMesh; }
	TObjectPtr<class USkeletalMeshComponent> GetFaceAccMesh() const { return FaceAccMesh; }
	TObjectPtr<class USkeletalMeshComponent> GetTopMesh() const { return TopMesh; }
	TObjectPtr<class USkeletalMeshComponent> GetBottomMesh() const { return BottomMesh; }
	TObjectPtr<class USkeletalMeshComponent> GetShoesMesh() const { return ShoesMesh; }


protected:
	//!< 메쉬 컴포넌트
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	// TObjectPtr<class USkeletalMeshComponent> HeadMesh;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	// TObjectPtr<class USkeletalMeshComponent> FaceMesh;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	// TObjectPtr<class USkeletalMeshComponent> TopMesh;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	// TObjectPtr<class USkeletalMeshComponent> BottomMesh;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	// TObjectPtr<class USkeletalMeshComponent> ShoesMesh;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	// TObjectPtr<class USkeletalMeshComponent> FaceAccMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<class USkeletalMeshComponent> HeadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<class USkeletalMeshComponent> FaceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<class USkeletalMeshComponent> TopMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<class USkeletalMeshComponent> BottomMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<class USkeletalMeshComponent> ShoesMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<class USkeletalMeshComponent> FaceAccMesh;

	UPROPERTY(ReplicatedUsing=OnRep_HeadMesh)
	TObjectPtr<class USkeletalMesh> HeadMeshReal;

	UPROPERTY(ReplicatedUsing=OnRep_FaceMesh)
	TObjectPtr<class USkeletalMesh> FaceMeshReal;

	UPROPERTY(ReplicatedUsing=OnRep_TopMesh)
	TObjectPtr<class USkeletalMesh> TopMeshReal;

	UPROPERTY(ReplicatedUsing=OnRep_BottomMesh)
	TObjectPtr<class USkeletalMesh> BottomMeshReal;

	UPROPERTY(ReplicatedUsing=OnRep_ShoesMesh)
	TObjectPtr<class USkeletalMesh> ShoesMeshReal;

	UPROPERTY(ReplicatedUsing=OnRep_FaceAccMesh)
	TObjectPtr<class USkeletalMesh> FaceAccMeshReal;

	#pragma region 매시 Get/Set
public:
	FORCEINLINE USkeletalMesh* GetHeadMeshT()		const { return HeadMeshReal;}
	FORCEINLINE USkeletalMesh* GetFaceMeshT()		const {	return FaceMeshReal;}
	FORCEINLINE USkeletalMesh* GetTopMeshT()			const {	return TopMeshReal;}
	FORCEINLINE USkeletalMesh* GetBottomMeshT()		const {	return BottomMeshReal;}
	FORCEINLINE USkeletalMesh* GetShoesMeshT()		const {	return ShoesMeshReal;}
	FORCEINLINE USkeletalMesh* GetFaceAccMeshT()		const {	return FaceAccMeshReal;}

	FORCEINLINE void UpdateHeadMesh()		const {	HeadMesh->SetSkeletalMesh(HeadMeshReal);	}
	FORCEINLINE void UpdateFaceMesh()		const {	FaceMesh->SetSkeletalMesh(FaceMeshReal);	}
	FORCEINLINE void UpdateTopMesh()		const {	TopMesh->SetSkeletalMesh(TopMeshReal);	}
	FORCEINLINE void UpdateBottomMesh()		const {	BottomMesh->SetSkeletalMesh(BottomMeshReal);	}
	FORCEINLINE void UpdateShoesMesh()		const {	ShoesMesh->SetSkeletalMesh(ShoesMeshReal);	}
	FORCEINLINE void UpdateFaceAccMesh()	const {	FaceAccMesh->SetSkeletalMesh(FaceAccMeshReal);	}

	FORCEINLINE void SetHeadMesh(USkeletalMesh* InMesh)			 {  HeadMeshReal = InMesh;}
	FORCEINLINE void SetFaceMesh(USkeletalMesh* InMesh)			 {	FaceMeshReal = InMesh;}
	FORCEINLINE void SetTopMesh(USkeletalMesh* InMesh)			 {	TopMeshReal = InMesh;}
	FORCEINLINE void SetBottomMesh(USkeletalMesh* InMesh)		 {	BottomMeshReal = InMesh;}
	FORCEINLINE void SetShoesMesh(USkeletalMesh* InMesh)		 {	ShoesMeshReal = InMesh;}
	FORCEINLINE void SetFaceAccMesh(USkeletalMesh* InMesh)		 {	FaceAccMeshReal = InMesh;}
#pragma endregion
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
	
protected:
	//!< 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

protected:
	/** 몽타주: 기본 공격  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member|Attack|Anim", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MeleeMontage;

	/** 플레그: 공격 시도 중 여부 */
	UPROPERTY()
	uint8 bAttackFlag : 1 = false;

	UFUNCTION(Server, Reliable)
	void ServerAttack();
	void PerformAttackTrace();

public:
	virtual void AttackHitCheck();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayAttackMontage();
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<class UACMoneyComponent> MoneyComp;

	//!< 상호작용 함수
public:
	/**
		@brief NearInteractables 배열에 상호작용 가능한(상호작용 컴포넌트를 지닌) 액터 저장. 상호작용 컴포넌트와 오버랩 시작시 불림.
		@param Interactor - 배열에 추가할 상호작용 가능한 액터
	**/
	void AddInteractable(AActor* Interactor);
	/**
		@brief NearInteractables 배열에 저장되어있는 액터 제거. 상호작용 컴포넌트와 오버랩 끝날시 불림.
		@param Interactor - 배열에서 제거할 상호작용 가능한 액터
	**/
	void RemoveInteractable(AActor* Interactor);

	//!< 상호작용 인터페이스 구현
protected:
	virtual bool CanInteract(AACCharacter* ACPlayer) override;
	virtual void OnInteract(AACCharacter* ACPlayer) override;

private:
	/**
		@brief  NearInteractables Array의 Actor들을 플레이어와 거리가 가까운 순서로 Sort. Sort 여부를 반환.
		@retval  - NearInteractables가 Sort되었으면 true, 아니면 false 반환
	**/
	bool SortNearInteractables();

 /**
     @brief 홀드 상호작용 관련 멤버변수를 모두 초기화
 **/
	void ResetHoldInteract();

 /**
     @brief  홀드 상호작용의 UI에 사용될 진행도
     @retval  - 진행도를 0.0~1.0 사이의 값으로 반환
 **/
	float GetHoldProgress() const;


	//!< 상호작용 멤버변수
public:
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TObjectPtr<class UACInteractableComponent> InteractBoxComponent;

private:
	TArray<AActor*> NearInteractables;

	AActor* CurrentHoldTarget;
	float CurrentHoldTime = 0.f;
	float RequiredHoldTime = 0.f;
	bool bIsHoldingInteract = false;


	// ===== 상점 관련 =====
  protected:
	  // 상점 컴포넌트는 유지 (아이템 장착 로직이 있으므로)
	  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop")
	  TObjectPtr<class UACShopComponent> ShopComponent;

	UFUNCTION()
	void OnRep_CharacterState();
public:
	void SetCharacterState(ECharacterState InCharacterState);
  protected:
	ESettingMode SettingMode = ESettingMode::None;
	
	FTimerHandle TimerHandle;
	
	
	UPROPERTY(ReplicatedUsing = OnRep_CharacterState, EditAnywhere, BlueprintReadWrite, Category = "State")
	ECharacterState CharacterState;
	

protected: // Dash 전용 맴버 변수
	FTimerHandle DashTimerHandle;

	UPROPERTY(Replicated)
	uint8 bDashCoolDown = true;
	
	
protected: // Sprint 전용 맴버 변수
	UPROPERTY(ReplicatedUsing = OnRep_Sprint)
	uint8 bSprint : 1 = false;
	
	UFUNCTION()
	void OnRep_Sprint();
	
	
	UFUNCTION()
	void GaugeUp();
	
	UFUNCTION()
	void GaugeDown();
	
	FTimerHandle SprintGaugeDownTimerHandle;
	FTimerHandle SprintGaugeUpTimerHandle;
	
	UPROPERTY(Replicated)
	int32 SprintGauge = 10;
	
protected:
	UPROPERTY(Replicated,EditAnywhere)
	TObjectPtr<class UACDestroyableStatComponent> Stat;
};
