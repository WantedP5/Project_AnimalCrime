
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

public:
	// ===== 입력 핸들러 (PlayerController가 호출) =====
	virtual void SetSteamFriendsList();
	virtual void SettingsClose() override;
	//virtual void GameReady();
};
