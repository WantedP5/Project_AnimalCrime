
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

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

public:
	// ===== 입력 핸들러 (PlayerController가 호출) =====
	virtual void SetSteamFriendsList();
	virtual void SettingsClose() override;

public:
	void UpdateHeadInfoName();
	void UpdateHeadInfoIcon();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	TObjectPtr<class UWidgetComponent> HeadInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	TObjectPtr<class UACBillboardComponent> Billboard;
};
