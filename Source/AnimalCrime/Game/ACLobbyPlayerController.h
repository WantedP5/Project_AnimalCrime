
#pragma once

#include "CoreMinimal.h"
#include "ACPlayerControllerBase.h"
#include "Game/ACGameEnums.h"
#include "ACLobbyPlayerController.generated.h"

UCLASS()
class ANIMALCRIME_API AACLobbyPlayerController : public AACPlayerControllerBase
{
	GENERATED_BODY()

public:
	AACLobbyPlayerController();
public:
	/**
		@brief 스팀 친구창 토글
		@param bVisible - true면 켜기, false면 끄기
	**/
	void SteamFriendListToggle(bool bVisible);
	void UpdatePlayerReadyText(int32 AllplayerNum);

public:
	/**
		@brief 게임모드로 맵이동 호출하는 함수
	**/
	UFUNCTION(Server, Reliable)
	void ServerStartGame();

	UFUNCTION(Server, Reliable)
	void ServerReadyToggle();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// ===== 입력 처리 핸들러 =====
protected:
	void HandleSteamFriendList(const struct FInputActionValue& Value);
	void HandleGameReady(const struct FInputActionValue& Value);

protected:

	UFUNCTION(Client, Reliable)
	void ClientPlayFadeIn();

public:
 /**
     @brief 게임시작 FadeIn 애니메이션 종료시 호출되는 함수
 **/
	UFUNCTION()
	void OnGameStartFadeInFinished();

public:
	bool GetIsReady() const { return bIsReady; }

	// ===== UI =====
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UACLobbyScreen> LobbyScreenClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UACLobbyScreen> LobbyScreen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> SteamFriendListClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UUserWidget> SteamFriendList;

	// ===== 입력 관련 =====
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> SteamFriendListAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> ReadyAction;

protected:

	bool bIsReady = false;
};
