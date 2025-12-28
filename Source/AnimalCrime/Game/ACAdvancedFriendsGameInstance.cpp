
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

#pragma region 엔진 제공 함수
void UACAdvancedFriendsGameInstance::Init()
{
	Super::Init();

	// 네트워크 끊김 처리
	GetEngine()->OnNetworkFailure().AddUObject(this, &UACAdvancedFriendsGameInstance::HandleNetworkFailure);
	GetEngine()->OnTravelFailure().AddUObject(this, &UACAdvancedFriendsGameInstance::HandleTravelFailure);

	// 세션이 Destroy 되었을 때 호출될 델리게이트 등록
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface.IsValid() == true)
	{
		OnDestroySessionCompleteHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(
			FOnDestroySessionCompleteDelegate::CreateUObject(this, &UACAdvancedFriendsGameInstance::OnDestroySessionComplete));
	}

	// 월드가 파괴되거나 변경될 때 호출되는 델리게이트 등록
	FWorldDelegates::OnWorldCleanup.AddUObject(this, &UACAdvancedFriendsGameInstance::OnWorldCleanup);

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

void UACAdvancedFriendsGameInstance::Shutdown()
{
	// Init에서 등록한 DestroySession 델리게이트 해제
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (SessionInterface.IsValid() && OnDestroySessionCompleteHandle.IsValid())
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteHandle);
	}

	UE_LOG(LogSY, Warning, TEXT("UACMainGameInstance::Shutdown"));

	Super::Shutdown();
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

	// 현재 맵을 변경된 맵으로 Update
	CurrentMapType = InMapType;
	
	// 먼저 Voice 정리
	CleanupVoiceSystem();

	// 모든 오디오 정리
	if (UWorld* World = GetWorld())
	{
		if (FAudioDevice* AudioDevice = World->GetAudioDeviceRaw())
		{
			// 모든 active sound 중지
			AudioDevice->StopAllSounds(true);
			// 오디오 컴포넌트들이 정리될 시간 확보
			AudioDevice->Flush(World);
		}
	}

	// 맵 변경 전에 보이스 먼저 종료. 
	// 서버호스트만 정리됨. 클라이언트는 OnWorldCleanup에서 정리.
	//IOnlineVoicePtr Voice = Online::GetVoiceInterface();
	//if (Voice.IsValid() == true)
	//{
	//	// 네트워크 보이스 중지
	//	Voice->StopNetworkedVoice(0);
	//	// 로컬 Talker 제거
	//	Voice->UnregisterLocalTalker(0);
	//	bVoiceInitialized = false;
	//}

	FTimerHandle TravelTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TravelTimerHandle,
		[this, InMapType]()
		{
			switch (InMapType)
			{
			case EMapType::Lobby:
				GetWorld()->ServerTravel("LobbyMap", true);
				break;
			case EMapType::Game:
				GetWorld()->ServerTravel("DemoMap", true);
				break;
			}
		},
		0.5f,  // 500ms 딜레이
		false
	);
}
#pragma endregion

void UACAdvancedFriendsGameInstance::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogSY, Warning, TEXT("Network Failure: %s"), *ErrorString);

	bVoiceInitialized = false;

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

	bVoiceInitialized = false;

	// 세션 정리
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(World);
	if (SessionInterface.IsValid() == true)
	{
		SessionInterface->DestroySession(NAME_GameSession);
	}
}

void UACAdvancedFriendsGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	bVoiceInitialized = false;
}

void UACAdvancedFriendsGameInstance::OnWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
	// 현재 작동 중인 월드가 정리될 때 실행됨 (서버/클라이언트 공통)
	if (World == GetWorld())
	{
		UE_LOG(LogSY, Warning, TEXT("World Cleanup Detected - Resetting Voice State"));

		IOnlineVoicePtr Voice = Online::GetVoiceInterface();
		if (Voice.IsValid() == true)
		{
			Voice->StopNetworkedVoice(0);
			Voice->UnregisterLocalTalker(0);
		}
		bVoiceInitialized = false;
	}
}

void UACAdvancedFriendsGameInstance::CleanupVoiceSystem()
{
	if (!bVoiceInitialized)
	{
		return;
	}

	IOnlineVoicePtr Voice = Online::GetVoiceInterface();
	if (Voice.IsValid())
	{
		UE_LOG(LogSY, Warning, TEXT("Cleaning up Voice System"));

		// 순서 중요: 먼저 네트워크 보이스 중지
		Voice->StopNetworkedVoice(0);

		// 약간의 딜레이 후 Talker 제거
		FPlatformProcess::Sleep(0.05f);

		Voice->UnregisterLocalTalker(0);

		bVoiceInitialized = false;

		UE_LOG(LogSY, Warning, TEXT("Voice System Cleaned"));
	}
}

void UACAdvancedFriendsGameInstance::TryStartVoice()
{
	UE_LOG(LogSY, Log, TEXT("Try Start Voice"));
	//만약 이미 보이스를 연결했다면 return.
	if (bVoiceInitialized == true)
	{
		UE_LOG(LogSY, Log, TEXT("Voice가 이미 연결되어있음"));
		return;
	}

	IOnlineVoicePtr Voice = Online::GetVoiceInterface();
	if (Voice.IsValid() == false)
	{
		UE_LOG(LogSY, Log, TEXT("Voice: VoiceInterface 없음"));
		return;
	}

	IOnlineSessionPtr Session = Online::GetSessionInterface(GetWorld());
	if (Session.IsValid() == false || Session->GetNamedSession(NAME_GameSession) == nullptr)
	{
		UE_LOG(LogSY, Log, TEXT("Voice: 세션 없음, Voice 시작 안 함"));
		return;
	}

	//보이스 연결
	Voice->RegisterLocalTalker(0);
	Voice->StartNetworkedVoice(0);
	bVoiceInitialized = true;
	UE_LOG(LogSY, Log, TEXT("Voice initialized"));
}
