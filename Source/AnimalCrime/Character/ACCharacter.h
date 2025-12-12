#pragma once

#include "CoreMinimal.h"
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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
 /**
     @brief  캐릭터 정보를 반환하는 함수. 캐릭터 베이스는 시민.
     @retval  - 캐릭터 정보 Enum
 **/
	virtual EACCharacterType GetCharacterType();

protected:
	virtual void BeginPlay() override;

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Interact(const FInputActionValue& Value); 
	virtual void ItemDrop(const FInputActionValue& Value);
	
	virtual void Attack();

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

	// 상점 위젯 토글 (서버 → 클라이언트 명령)
	UFUNCTION(Client, Reliable)
	void ClientToggleShopWidget(TSubclassOf<class UACShopWidget> WidgetClass);

private:
	// 현재 열려있는 상점 위젯
	UPROPERTY()
	TObjectPtr<class UACShopWidget> CurrentShopWidget;

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
	//!< 키 입력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ItemDropAction;
	
	/** Input Action: 기본 공격 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member|Attack|Input", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MeleeAction;
	
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


protected:
	// 상점 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop")
	TObjectPtr<class UACShopComponent> ShopComponent;
};
