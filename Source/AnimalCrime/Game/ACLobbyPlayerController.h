
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Game/ACGameEnums.h"
#include "ACLobbyPlayerController.generated.h"

UCLASS()
class ANIMALCRIME_API AACLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AACLobbyPlayerController();
public:
	/**
		@brief 입력 모드 변경 (기본 입력 ↔ 설정 메뉴 입력)
		@param NewMode - 새로운 입력 모드
	**/
	void ChangeInputMode(EInputMode NewMode);

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
	void HandleMove(const struct FInputActionValue& Value);
	void HandleLook(const struct FInputActionValue& Value);
	void HandleJump(const struct FInputActionValue& Value);
	void HandleStopJumping(const struct FInputActionValue& Value);
	void HandleInteractStart(const struct FInputActionValue& Value);
	void HandleInteractHold(const struct FInputActionValue& Value);
	void HandleInteractRelease(const struct FInputActionValue& Value);
	void HandleItemDrop(const struct FInputActionValue& Value);
	void HandleAttack(const struct FInputActionValue& Value);
	void HandleSettingsClose(const struct FInputActionValue& Value);
	void HandleSteamFriendList(const struct FInputActionValue& Value);
	void HandleGameReady(const struct FInputActionValue& Value);

	// ===== UI =====
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UACLobbyScreen> LobbyScreenClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UACLobbyScreen> LobbyScreen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> FadeInScreenClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UUserWidget> FadeInScreen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> SteamFriendListClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UUserWidget> SteamFriendList;

	// ===== 입력 관련 =====
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> ItemDropAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> MeleeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> SettingsMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> SettingsCloseAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> SteamFriendListAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> ReadyAction;

protected:

	bool bIsReady = false;
};
