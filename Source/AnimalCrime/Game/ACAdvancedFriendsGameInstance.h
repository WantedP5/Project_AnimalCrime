
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
	virtual void Shutdown() override;
	virtual void OnStart() override;
#pragma endregion

#pragma region Map Level 
public:
	void LoadLobbyMap();
	void LoadGameMap();

	EMapType GetMapType() const;

private:
	void UpdateMap(EMapType InMapType);

	void BeginLoadingScreen(const FString& MapName);
	void OnSeamlessTravelStart(UWorld* CurrentWorld, const FString& LevelName);

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
	UFUNCTION()
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	/**
		@brief 월드(레벨)가 파괴되거나 전환될 때 호출되는 콜백 함수
		@param World             - 현재 정리 중인 UWorld 객체
		@param bSessionEnded     - 게임 세션이 종료되었는지를 나타내는 플래그. true이면 플레이 세션이 종료된 상태, false면 단순 레벨 전환
		@param bCleanupResources - 월드 관련 리소스를 정리할지 여부를 나타내는 플래그. true이면 월드에 연결된 컴포넌트, Audio, VoIP 등 리소스를 강제로 정리해야함.
	**/
	//UFUNCTION()
	//void OnWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources);

	/**
		@brief Voice 정리
	**/
	//void CleanupVoiceSystem();
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
	UPROPERTY(EditDefaultsOnly, Category = "Loading")
	TSubclassOf<UUserWidget> BlackScreenClass;

	bool bVoiceInitialized = false;

	int32 NumClientsReady = 0;
};

