#pragma once

#include "CoreMinimal.h"
#include "Character/ACCharacter.h"
#include "ACMafiaCharacter.generated.h"

UCLASS()
class ANIMALCRIME_API AACMafiaCharacter : public AACCharacter
{
	GENERATED_BODY()

public:
	AACMafiaCharacter();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	//!< 상호작용 인터페이스
protected:
	virtual bool CanInteract(AACCharacter* ACPlayer) override;
	virtual void OnInteract(AACCharacter* ACPlayer) override;

public:
/**
	@brief 클라이언트에서 폭탄 설치 가능 영역 표시 여부를 갱신함.
	@param bVisible - true면 설치구역을 보여주고, false면 숨김
**/
	UFUNCTION(Client, Reliable)
	void ClientSetBombAreaVisible(bool bVisible);

	UFUNCTION(Client, Reliable)
	void ClientSetEscapeAreaVisible(bool bVisible);

	virtual EACCharacterType GetCharacterType() override;
protected:

	void ItemDrop() override;

	virtual void ServerItemDrop_Implementation() override;

	UFUNCTION()
	void OnRep_HandBomb();

	virtual void AttackHitCheck() override;
public:
	//!<아이템
	UPROPERTY(ReplicatedUsing = OnRep_HandBomb)
	TObjectPtr<class AACEscapeMissionBomb> HandBomb;

};
