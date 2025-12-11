
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ACInteractInterface.h"
#include "ACEscapeMissionBomb.generated.h"

UCLASS()
class ANIMALCRIME_API AACEscapeMissionBomb : public AActor, public IACInteractInterface
{
	GENERATED_BODY()

public:
	AACEscapeMissionBomb();

protected:
	virtual void BeginPlay() override;
	/**
	@brief 어떤 변수를 네트워크로 복제할지 지정하는 함수
	@param OutLifetimeProps -
	**/
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	//UFUNCTION()
	//void OnInteractTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	//void OnInteractTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnRep_AttachedCharacter();

public:
	FORCEINLINE UStaticMeshComponent* GetBombMeshComp() const { return BombMeshComp; } 

	void AttachToCharacter();
	void DetachFromCharacter();

	FORCEINLINE TObjectPtr<class UBoxComponent> GetRootComp() const { return RootComp; }

protected:
	virtual bool CanInteract(class AACCharacter* Interactor) override;		// 누가 상호작용 가능한지(캐릭터 타입 체크) |
	virtual void OnInteract(class AACCharacter* Interactor) override;		// 실제 상호작용 로직(서버에서 실행) |


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class UBoxComponent> RootComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> BombMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TObjectPtr<class UACInteractableComponent> InteractBoxComponent;

public:
	//!< 누가 이 폭탄을 들고 있는지 저장하는 변수
	UPROPERTY(ReplicatedUsing = OnRep_AttachedCharacter)
	TObjectPtr<class AACTestMafiaCharacter> AttachedCharacter;
};
