
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
	virtual void OnInteract(class AACCharacter* ACPlayer, EInteractionKey InKey) override;
	virtual EACInteractorType GetInteractorType() const override;

	//=== 위젯 인터페이스 ===
	virtual class UWidgetComponent* GetInteractionWidget() const override;
	virtual void ShowInteractionHints(const TArray<class UACInteractionData*>& Interactions) override;
	virtual void HideInteractionHints() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class UBoxComponent> RootComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> BombMeshComp;

	//!< 상호작용 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TObjectPtr<class UACInteractableComponent> InteractBoxComponent;

	//!< 상호작용 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact")
	TObjectPtr<class UWidgetComponent> InteractionWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TSubclassOf<class UACInteractionInfoWidget> InteractionInfoWidgetClass;

public:
	//!< 누가 이 폭탄을 들고 있는지 저장하는 변수
	UPROPERTY(ReplicatedUsing = OnRep_AttachedCharacter)
	TObjectPtr<class AACMafiaCharacter> AttachedCharacter;
};
