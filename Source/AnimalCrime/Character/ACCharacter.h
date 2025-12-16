#pragma once

#include "CoreMinimal.h"
#include "Game/ACGameEnums.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
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

public:
 /**
     @brief 현재 입력매핑 컨텍스트를 전부 지우고 새로운 입력매핑 컨텍스트로 바꾸는 함수
     @param NewMode - 입력모드 Enum
 **/
	void ChangeInputMode(EInputMode NewMode);

	// ===== 입력 핸들러 (PlayerController가 호출) =====
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Interact(const FInputActionValue& Value);
	virtual void ItemDrop(const FInputActionValue& Value);
	virtual void Attack();
	virtual void SettingsClose(const FInputActionValue& Value);

protected:

	UFUNCTION(Server, Reliable)
	void ServerInteract();

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
	void AttackHitCheck();
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayAttackMontage();


	
public:
	void AddInteractable(AActor* Interactor);
	void RemoveInteractable(AActor* Interactor);
protected:
	virtual bool CanInteract(AACCharacter* ACPlayer) override;		// 누가 상호작용 가능한지(캐릭터 타입 체크) |
	virtual void OnInteract(AACCharacter* ACPlayer) override;		// 실제 상호작용 로직(서버에서 실행) |
	virtual FString GetInteractableName() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TObjectPtr<class UACInteractableComponent> InteractBoxComponent;
private:
	bool SortNearInteractables();
private:
	TArray<AActor*> NearInteractables;

	// ===== 상점 관련 =====
  protected:
	  // 상점 컴포넌트는 유지 (아이템 장착 로직이 있으므로)
	  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop")
	  TObjectPtr<class UACShopComponent> ShopComponent;

  protected:
	  ESettingMode SettingMode = ESettingMode::None;
};
