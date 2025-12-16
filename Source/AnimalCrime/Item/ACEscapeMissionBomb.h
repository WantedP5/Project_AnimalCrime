
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

	UFUNCTION()
	void OnRep_AttachedCharacter();

public:
	FORCEINLINE UStaticMeshComponent* GetBombMeshComp() const { return BombMeshComp; } 

	void AttachToCharacter();
	void DetachFromCharacter();

	FORCEINLINE TObjectPtr<class UBoxComponent> GetRootComp() const { return RootComp; }

	//!< 상호작용 인터페이스
protected:
	virtual bool CanInteract(class AACCharacter* ACPlayer) override;
	virtual void OnInteract(class AACCharacter* ACPlayer) override;
	virtual FString GetInteractableName() const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class UBoxComponent> RootComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> BombMeshComp;

	//!< 상호작용 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TObjectPtr<class UACInteractableComponent> InteractBoxComponent;

public:
	//!< 누가 이 폭탄을 들고 있는지 저장하는 변수
	UPROPERTY(ReplicatedUsing = OnRep_AttachedCharacter)
	TObjectPtr<class AACTestMafiaCharacter> AttachedCharacter;
};
