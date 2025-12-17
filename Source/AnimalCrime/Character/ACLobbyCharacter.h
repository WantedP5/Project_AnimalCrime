
#pragma once

#include "CoreMinimal.h"
#include "Character/ACCharacter.h"
#include "ACLobbyCharacter.generated.h"


UCLASS()
class ANIMALCRIME_API AACLobbyCharacter : public AACCharacter
{
	GENERATED_BODY()

public:
	AACLobbyCharacter();

	// ===== 입력 핸들러 (PlayerController가 호출) =====
	virtual void SetSteamFriendsList(const FInputActionValue& Value);
	virtual void SettingsClose(const FInputActionValue& Value) override;
	virtual void GameReady(const FInputActionValue& Value);
};
