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
	virtual void BeginPlay() override;

protected:
	virtual bool CanInteract(AACCharacter* Interactor) override;		// 누가 상호작용 가능한지(캐릭터 타입 체크) |
	virtual void OnInteract(AACCharacter* Interactor) override;		// 실제 상호작용 로직(서버에서 실행) |
	virtual FString GetInteractableName() const override;
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

	void ItemDrop(const FInputActionValue& Value) override;

	virtual void ServerItemDrop_Implementation() override;

	UFUNCTION()
	void OnRep_HandBomb();

public:
	//!<아이템
	UPROPERTY(ReplicatedUsing = OnRep_HandBomb)
	TObjectPtr<class AACEscapeMissionBomb> HandBomb;

};
