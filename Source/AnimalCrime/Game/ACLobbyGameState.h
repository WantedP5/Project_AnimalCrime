
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
	/**
		@brief 준비한 플레이어 수 / 전체 플레이어 수를 갱신하고 UI에 알림
	**/
	void UpdateReadyPlayer();

	/**
		@brief 플레이어의 준비 상태를 설정
		@param PS     - @brief 플레이어의 준비 상태를 설정
		@param bReady - @brief 플레이어의 준비 상태를 설정
	**/
	void SetReadyPlayer(APlayerState* PS, bool bReady);

	/**
		@brief  PlayerState가 호스트인지 확인하는 함수
		@param  PS - 확인할 PlayerState
		@retval    - 호스트면 true, 아니면 false
	**/
	bool IsHostPlayer(APlayerState* PS);

	/** 해당 PlayerState가 준비 완료 상태인지 여부 */
	UFUNCTION(BlueprintPure)
	bool IsPlayerReady(const APlayerState* PlayerState) const;

	UFUNCTION()
	void OnRep_PlayerCount();

	UFUNCTION()
	void OnRep_ReadyPlayerArray();

	UPROPERTY()
	FOnPlayerCountChanged OnPlayerCountChanged;

	FORCEINLINE int32 GetAllPlayerCount() { return AllPlayerCount; }
	FORCEINLINE int32 GetReadyPlayerCount() { return ReadyPlayerCount; }
	FORCEINLINE APlayerState* GetHostPlayerState() const { return HostPlayerState; }
protected:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerCount)
	int32 AllPlayerCount = 0;
	UPROPERTY(Replicated)
	int32 ReadyPlayerCount = 0;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_ReadyPlayerArray)
	TArray<TObjectPtr<APlayerState>> ReadyPlayerArray;

protected:
	UPROPERTY(Replicated)
	TObjectPtr<APlayerState> HostPlayerState;
};
