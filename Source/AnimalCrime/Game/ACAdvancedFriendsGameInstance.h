
#pragma once

#include "CoreMinimal.h"
#include "ACGameEnums.h"
#include "AdvancedFriendsGameInstance.h"
#include "ACAdvancedFriendsGameInstance.generated.h"

UENUM(BlueprintType)
enum class EMapType : uint8
{
	None		UMETA(DisplayName = "None"),
	Lobby       UMETA(DisplayName = "Lobby"),
	Game        UMETA(DisplayName = "Game"),
	Result      UMETA(DisplayName = "Result"),
	Shop	    UMETA(DisplayName = "Shop"),
	Unknown     UMETA(DisplayName = "Unknown")
};

UCLASS()
class ANIMALCRIME_API UACAdvancedFriendsGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()

#pragma region 엔진 제공 함수
public:
	virtual void Init() override;
	//virtual void Shutdown() override;
	virtual void OnStart() override;
#pragma endregion

#pragma region Map Level 
public:
	void LoadLobbyMap();
	void LoadGameMap();

	EMapType GetMapType() const;

private:
	void UpdateMap(EMapType InMapType);

	//void BeginLoadingScreen(const FString& MapName);
	//void OnSeamlessTravelStart(UWorld* CurrentWorld, const FString& LevelName);

#pragma endregion

public:
	/** 실제 세션 생성은 BP에서 구현 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Session")
	void CreateSession();

protected:

	/**
		@brief 네트워크 실패 시 처리 로직
		@param World       - 어느 월드에서 발생했는지
		@param NetDriver   - 어떤 네트워크 드라이버인지
		@param FailureType - 실패 타입 (연결 끊김, 타임아웃 등)
		@param ErrorString - 에러 메시지
	**/
	UFUNCTION()
	void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	/**
		@brief 맵 이동 실패 시 처리 로직
		@param World       - 어느 월드에서 발생했는지
		@param FailureType - 실패 타입
		@param ErrorString - 에러 메시지
	**/
	UFUNCTION()
	void HandleTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString);

public:
	/**
		@brief 음성 채팅(보이스)을 시작하기 위한 초기화 시도 함수
	**/
	void TryStartVoice();

	void OnClientVoiceCleanupFinished();
private:
	void DoServerTravel();
private:
	/**
		@brief 온라인 세션이 삭제가 완료되었을 때 호출되는 콜백 함수
		@param SessionName    - 삭제된 세션의 이름
		@param bWasSuccessful - 세션 삭제 성공 여부 (true = 성공, false = 실패)
	**/
	//UFUNCTION()
	//void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	void CheckServerVoiceCleanup();
	void CheckFinalStateBeforeTravel();

private:
	FDelegateHandle OnDestroySessionCompleteHandle;

#pragma region Map Level 관련 맴버 변수 
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	EMapType CurrentMapType;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	FString LobbyMapName;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	FString GameMapName;
#pragma endregion

public:
	//!< 플레이어 역할 저장
	TMap<FUniqueNetIdRepl, EPlayerRole> SavedPlayerRoles;

private:
	int32 NumClientsReady = 0;

	int32 ServerCleanupPollingAttempts = 0;
	int32 FinalCheckPollingAttempts = 0;
	int32 ConsecutiveCleanPolls = 0; // 연속으로 컴포넌트가 없었던 횟수
	bool bServerVoiceCleaned = false;
	int32 DoServerTravelRetryCount = 0; // DoServerTravel 재시도 횟수

	static constexpr int32 MaxServerCleanupPollingAttempts = 100; // 최대 1초
	static constexpr int32 MaxFinalCheckPollingAttempts = 100; // 최대 1초
	static constexpr int32 RequiredCleanPolls = 20; // 연속 20번(200ms) 깨끗해야 완료
	static constexpr float ServerPollingInterval = 0.01f; // 10ms
	static constexpr int32 MaxDoServerTravelRetry = 50; // DoServerTravel 최대 재시도 (500ms)
};

