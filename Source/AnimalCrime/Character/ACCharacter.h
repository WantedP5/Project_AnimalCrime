#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ACCharacter.generated.h"

UCLASS()
class ANIMALCRIME_API AACCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AACCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Interact(const FInputActionValue& Value);
	virtual void ItemDrop(const FInputActionValue& Value);
	
	virtual void Attack();

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
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayAttackMontage();

protected:
	// 상점 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop")
	TObjectPtr<class UACShopComponent> ShopComponent;
};
