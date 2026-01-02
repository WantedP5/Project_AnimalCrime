
#include "ACAdvancedFriendsGameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "AdvancedSessionsLibrary.h"
#include "CreateSessionCallbackProxyAdvanced.h"
#include "Engine/NetDriver.h"
#include "Engine/Engine.h"
#include "AudioDevice.h"
#include "AnimalCrime.h"
#include "ACPlayerControllerBase.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"

#pragma region 엔진 제공 함수
void UACAdvancedFriendsGameInstance::Init()
{
	Super::Init();

	// 네트워크 끊김 처리
	GetEngine()->OnNetworkFailure().AddUObject(this, &UACAdvancedFriendsGameInstance::HandleNetworkFailure);
	GetEngine()->OnTravelFailure().AddUObject(this, &UACAdvancedFriendsGameInstance::HandleTravelFailure);

	// 맵 관련 초기화
	CurrentMapType = EMapType::None;

	LobbyMapName = TEXT("LobbyMap");
	GameMapName = TEXT("henaMap");

	// PIE(에디터 플레이) 환경에서도 SeamlessTravel 허용
	UE_LOG(LogTemp, Warning, TEXT("UACMainGameInstance::Init"));
	if (GEngine)
	{
		GEngine->Exec(GetWorld(), TEXT("net.AllowPIESeamlessTravel 1"));
	}

}

void UACAdvancedFriendsGameInstance::OnStart()
{
	Super::OnStart();

	UE_LOG(LogSY, Warning, TEXT("UACMainGameInstance::OnStart"));
}
#pragma endregion

#pragma region Map Level
void UACAdvancedFriendsGameInstance::LoadLobbyMap()
{
	UpdateMap(EMapType::Lobby);
}

void UACAdvancedFriendsGameInstance::LoadGameMap()
{
	UpdateMap(EMapType::Game);
}

EMapType UACAdvancedFriendsGameInstance::GetMapType() const
{
	return CurrentMapType;
}

void UACAdvancedFriendsGameInstance::UpdateMap(const EMapType InMapType)
{
	if (CurrentMapType == InMapType)
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentMapType == InMapType"));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// 현재 맵을 변경된 맵으로 Update
	CurrentMapType = InMapType;
	NumClientsReady = 0;

	// Server Voice 정리 (TObjectIterator 사용 안 함 - Voice 인터페이스만 사용)
	if (World->GetNetMode() != NM_Client)
	{
		UE_LOG(LogSY, Log, TEXT("=== Server Voice 정리 시작 ==="));

		IOnlineVoicePtr Voice = Online::GetVoiceInterface();
		if (Voice.IsValid())
		{
			Voice->ClearVoicePackets();
			Voice->RemoveAllRemoteTalkers();
			Voice->StopNetworkedVoice(0);
			Voice->UnregisterLocalTalker(0);
			UE_LOG(LogSY, Log, TEXT("Server Voice 정리 완료"));
		}

		// Audio Flush
		if (FAudioDeviceHandle AudioDeviceHandle = World->GetAudioDevice())
		{
			AudioDeviceHandle->StopAllSounds(true);
			AudioDeviceHandle->Flush(World);
			UE_LOG(LogSY, Log, TEXT("Server Audio Flush 완료"));
		}

		UE_LOG(LogSY, Log, TEXT("=== Server Voice 정리 완료 ==="));
	}

	// Client들에게 Voice 정리 요청
	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		AACPlayerControllerBase* PC = Cast<AACPlayerControllerBase>(It->Get());
		if (PC == nullptr)
		{
			continue;
		}
		PC->Client_CleanupVoiceBeforeTravel();
	}
}
#pragma endregion

void UACAdvancedFriendsGameInstance::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogSY, Warning, TEXT("Network Failure: %s"), *ErrorString);

	// 세션 정리
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(World);
	if (SessionInterface.IsValid() == true)
	{
		SessionInterface->DestroySession(NAME_GameSession);
	}
}

void UACAdvancedFriendsGameInstance::HandleTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogSY, Warning, TEXT("Travel Failure: %s"), *ErrorString);

	// 세션 정리
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(World);
	if (SessionInterface.IsValid() == true)
	{
		SessionInterface->DestroySession(NAME_GameSession);
	}
}

void UACAdvancedFriendsGameInstance::OnClientVoiceCleanupFinished()
{
	UE_LOG(LogSY, Log, TEXT("OnClientVoiceCleanupFinished!!"));
	++NumClientsReady;

	// 모든 Client가 정리를 완료했는가?
	if (NumClientsReady >= GetWorld()->GetNumPlayerControllers())
	{
		UE_LOG(LogSY, Log, TEXT("=== ✅ All clients finished cleanup ==="));

		// 약간의 안전 딜레이 후 Travel
		FTimerHandle DelayTimer;
		GetWorld()->GetTimerManager().SetTimer(DelayTimer,
			this, &UACAdvancedFriendsGameInstance::DoServerTravel,
			0.2f, false);  // 200ms 대기
	}
}

void UACAdvancedFriendsGameInstance::DoServerTravel()
{
	UE_LOG(LogSY, Log, TEXT("DoServerTravel!!"));

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UE_LOG(LogSY, Log, TEXT("Safe to travel"));

	switch (CurrentMapType)
	{
	case EMapType::Lobby:
		World->ServerTravel("LobbyMap", true);
		break;
	case EMapType::Game:
		World->ServerTravel("DemoMap", true);
		break;
	}
}
