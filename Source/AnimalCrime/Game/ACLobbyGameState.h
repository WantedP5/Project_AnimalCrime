
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ACLobbyGameState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerCountChanged, int32, ReadyPlayerCount, int32, AllPlayerCount);

UCLASS()
class ANIMALCRIME_API AACLobbyGameState : public AGameState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
 /**
     @brief 플레이어가 들어오면 호출됨
     @param PlayerState - 새로운 플레이어의 플레이어 스테이트
 **/
	virtual void AddPlayerState(APlayerState* PlayerState) override;
 /**
     @brief 플레이어가 제거되면 호출됨
     @param PlayerState - 삭제된 플레이어의 플레이어 스테이트
 **/
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

public:
	void UpdateReadyPlayer();

	UFUNCTION()
	void OnRep_PlayerCount();

	UPROPERTY()
	FOnPlayerCountChanged OnPlayerCountChanged;

	FORCEINLINE int32 GetAllPlayerCount() { return AllPlayerCount; }
	FORCEINLINE int32 GetReadyPlayerCount() { return ReadyPlayerCount; }
protected:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerCount)
	int32 AllPlayerCount = 0;
	UPROPERTY(ReplicatedUsing = OnRep_PlayerCount)
	int32 ReadyPlayerCount = 0;
};
