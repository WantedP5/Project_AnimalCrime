
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

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UACLobbyScreen> LobbyScreenClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UACLobbyScreen> LobbyScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> SteamFriendListClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UUserWidget> SteamFriendList;
};
