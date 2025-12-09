#pragma once

#include "CoreMinimal.h"
#include "Character/ACCharacter.h"
#include "ACTestMafiaCharacter.generated.h"

UCLASS()
class ANIMALCRIME_API AACTestMafiaCharacter : public AACCharacter
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	void Interact(const FInputActionValue& Value) override;

	void ItemDrop(const FInputActionValue& Value) override;

	UFUNCTION(Server, Reliable)
	void ServerInteract();

	virtual void ServerItemDrop_Implementation() override;

	UFUNCTION()
	void OnRep_HandBomb();

public:
	//!<아이템
	UPROPERTY()
	TObjectPtr<class AACEscapeMissionBomb> InteractBomb;
	UPROPERTY(ReplicatedUsing = OnRep_HandBomb)
	TObjectPtr<class AACEscapeMissionBomb> HandBomb;
};
