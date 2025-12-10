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

public:
/**
	@brief 클라이언트에서 폭탄 설치 가능 영역 표시 여부를 갱신함.
	@param bVisible - true면 설치구역을 보여주고, false면 숨김
**/
	UFUNCTION(Client, Reliable)
	void ClientSetBombAreaVisible(bool bVisible);
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
	UPROPERTY()
	TObjectPtr<class AACTestBlackMarketDealer> InteractDealer;

};
