
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ACLobbyPlayerController.generated.h"

UCLASS()
class ANIMALCRIME_API AACLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AACLobbyPlayerController();

 /**
     @brief 스팀 친구창 토글
     @param bVisible - true면 켜기, false면 끄기
 **/
	void SteamFriendListToggle(bool bVisible);
	void UpdatePlayerReadyText(int32 AllplayerNum);
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// ===== 입력 처리 핸들러 =====
protected:
	void HandleSteamFriendList(const struct FInputActionValue& Value);
	void HandleGameReady(const struct FInputActionValue& Value);

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
};
