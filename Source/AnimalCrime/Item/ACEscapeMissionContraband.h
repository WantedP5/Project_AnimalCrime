

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ACInteractInterface.h"
#include "ACEscapeMissionContraband.generated.h"

UCLASS()
class ANIMALCRIME_API AACEscapeMissionContraband : public AActor, public IACInteractInterface
{
	GENERATED_BODY()

public:
	AACEscapeMissionContraband();

protected:
	virtual void BeginPlay() override;

	//!< 상호작용 인터페이스
protected:
	//virtual bool CanInteract(class AACCharacter* ACPlayer) override;
	virtual void OnInteract(class AACCharacter* ACPlayer, EInteractionKey InKey) override;
	//virtual float GetRequiredHoldTime() const override;
	virtual EACInteractorType GetInteractorType() const override;

	//=== 위젯 인터페이스 ===
	virtual class UWidgetComponent* GetInteractionWidget() const override;
	virtual void ShowInteractionHints(const TArray<class UACInteractionData*>& Interactions) override;
	virtual void HideInteractionHints() override;

protected:
	/** Root Collision */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USphereComponent> Sphere;

	/** Niagara Effect */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UNiagaraComponent> NiagaraEffect;

protected:
	//!< 상호작용 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TObjectPtr<class UACInteractableComponent> InteractBoxComponent;

	//!< 상호작용 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact")
	TObjectPtr<class UWidgetComponent> InteractionWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TSubclassOf<class UACInteractionInfoWidget> InteractionInfoWidgetClass;
};
