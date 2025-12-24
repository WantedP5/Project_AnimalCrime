
#pragma once

#include "CoreMinimal.h"
#include "ACGameEnums.h"
#include "Character/ACCharacter.h"
#include "GameFramework/PlayerState.h"
#include "ACPlayerState.generated.h"

UCLASS()
class ANIMALCRIME_API AACPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/**
		@brief 관전 상태로 전환하고, 자신을 관전대상 폰 목록에서 제거함.
	**/
	void EnterSpectatorState();

public:
	//!< 플레이어 역할
	UPROPERTY(Replicated)
	EPlayerRole PlayerRole = EPlayerRole::Police;

	//!< 플레이어 상태
	UPROPERTY(Replicated)
	ECharacterState CharacterState = ECharacterState::Free;

};
