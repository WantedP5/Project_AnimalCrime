#pragma once

#include "CoreMinimal.h"
#include "Game/ACGameEnums.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interface/ACInteractInterface.h"
#include "ACCharacter.generated.h"

UENUM()
enum class ECharacterState : uint8
{
	None,
	Free,		
	OnDamage,
	Stun,
	Prison,
	MAX_COUNT
};

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
	virtual void SettingsClose(const FInputActionValue& Value);
	virtual void Jump() override;
	virtual void SettingsClose();

protected:

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* Target);

	UFUNCTION(Server, Reliable)
	virtual void ServerItemDrop();

public:

	UFUNCTION(BlueprintCallable)
	bool CheckProcessAttack() const;
	UFUNCTION(BlueprintCallable)
	void ChangeAttackTrue();
	UFUNCTION(BlueprintCallable)
	void ChangeAttackFalse();
	

public:
	TObjectPtr<class USkeletalMeshComponent> GetHeadMesh() const { return HeadMesh; }
	TObjectPtr<class USkeletalMeshComponent> GetFaceAccMesh() const { return FaceAccMesh; }
	TObjectPtr<class USkeletalMeshComponent> GetTopMesh() const { return TopMesh; }
	TObjectPtr<class USkeletalMeshComponent> GetBottomMesh() const { return BottomMesh; }
	TObjectPtr<class USkeletalMeshComponent> GetShoesMesh() const { return ShoesMesh; }


protected:
	//!< 메쉬 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> HeadMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> FaceMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> TopMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> BottomMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> ShoesMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> FaceAccMesh;

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
  protected:
	  ESettingMode SettingMode = ESettingMode::None;
	
	
	UPROPERTY(ReplicatedUsing = OnRep_CharacterState, EditAnywhere, BlueprintReadWrite, Category = "State")
	ECharacterState CharacterState;
};
