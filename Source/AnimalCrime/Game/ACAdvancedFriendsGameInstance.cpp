
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
#include "Net/VoiceConfig.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "ACPlayerControllerBase.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"
#include "VoipListenerSynthComponent.h"

#pragma region 엔진 제공 함수
void UACAdvancedFriendsGameInstance::Init()
{
	Super::Init();

	// 네트워크 끊김 처리
	GetEngine()->OnNetworkFailure().AddUObject(this, &UACAdvancedFriendsGameInstance::HandleNetworkFailure);
	GetEngine()->OnTravelFailure().AddUObject(this, &UACAdvancedFriendsGameInstance::HandleTravelFailure);

	// 세션이 Destroy 되었을 때 호출될 델리게이트 등록
	//IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	//if (SessionInterface.IsValid() == true)
	//{
	//	OnDestroySessionCompleteHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(
	//		FOnDestroySessionCompleteDelegate::CreateUObject(this, &UACAdvancedFriendsGameInstance::OnDestroySessionComplete));
	//}

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

//void UACAdvancedFriendsGameInstance::Shutdown()
//{
//	// Init에서 등록한 DestroySession 델리게이트 해제
//	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
//	if (SessionInterface.IsValid() && OnDestroySessionCompleteHandle.IsValid())
//	{
//		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteHandle);
//	}
//
//	UE_LOG(LogSY, Warning, TEXT("UACMainGameInstance::Shutdown"));
//
//	Super::Shutdown();
//}

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
	NumClientsReady = 0;
	bServerVoiceCleaned = false;
	DoServerTravelRetryCount = 0;

	// ★ Server Voice를 먼저 중지해서 Client들 사이의 Voice 패킷 전달을 차단
	// 이렇게 해야 Client들이 정리하는 동안 새로운 VoipListenerSynthComponent가 생성되지 않음
	if (GetWorld()->GetNetMode() != NM_Client)
	{
		UE_LOG(LogSY, Log, TEXT("=== Server Voice 먼저 중지 시작 ==="));

		IOnlineVoicePtr Voice = Online::GetVoiceInterface();
		if (Voice.IsValid())
		{
			// ★ Voice 패킷 버퍼 먼저 비우기 - 버퍼에 남아있는 패킷이 처리되어 새 컴포넌트가 생성되는 것 방지
			Voice->ClearVoicePackets();
			Voice->RemoveAllRemoteTalkers();
			Voice->StopNetworkedVoice(0);
			Voice->UnregisterLocalTalker(0);
			UE_LOG(LogSY, Log, TEXT("Server Voice 중지 완료"));
		}

		// Server의 VoipListenerSynthComponent도 먼저 정리
		for (TObjectIterator<UVoipListenerSynthComponent> It; It; ++It)
		{
			UVoipListenerSynthComponent* VoipComp = *It;
			if (IsValid(VoipComp))
			{
				UE_LOG(LogSY, Log, TEXT("Server - Destroying VoipComp: %s"), *VoipComp->GetName());
				VoipComp->Stop();
				if (UAudioComponent* AudioComp = VoipComp->GetAudioComponent())
				{
					AudioComp->Stop();
					if (AudioComp->IsRegistered())
					{
						AudioComp->UnregisterComponent();
					}
					AudioComp->DestroyComponent();
				}
				if (VoipComp->IsRegistered())
				{
					VoipComp->UnregisterComponent();
				}
				VoipComp->DestroyComponent();
			}
		}

		// Audio Flush
		if (FAudioDeviceHandle AudioDeviceHandle = GetWorld()->GetAudioDevice())
		{
			AudioDeviceHandle->StopAllSounds(true);
			AudioDeviceHandle->Flush(GetWorld());
			UE_LOG(LogSY, Log, TEXT("Server Audio Flush 완료"));
		}

		UE_LOG(LogSY, Log, TEXT("=== Server Voice 먼저 중지 완료 ==="));
	}

	// 그 다음 Client들에게 Voice 정리 요청
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
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

//void UACAdvancedFriendsGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
//{
//}

void UACAdvancedFriendsGameInstance::CheckServerVoiceCleanup()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogSY, Error, TEXT("World is null in server polling"));
		bServerVoiceCleaned = true;
		CheckFinalStateBeforeTravel();
		return;
	}

	// ★ Voice 패킷 버퍼 계속 비우기 - 지연된 패킷이 새 컴포넌트를 생성하는 것 방지
	IOnlineVoicePtr Voice = Online::GetVoiceInterface();
	if (Voice.IsValid())
	{
		Voice->ClearVoicePackets();
	}

	// VoipListenerSynthComponent 및 내부 AudioComponent 직접 찾아서 정리 (Transient 패키지 포함)
	// UE-146893, UE-169798 버그 대응: DestroyComponent()로 강제 파괴
	bool bServerVoipExists = false;
	int32 FoundCount = 0;

	for (TObjectIterator<UVoipListenerSynthComponent> It; It; ++It)
	{
		UVoipListenerSynthComponent* VoipComp = *It;
		if (IsValid(VoipComp))
		{
			bServerVoipExists = true;
			FoundCount++;

			UE_LOG(LogSY, Log, TEXT("Server cleanup - Found VoipComp: %s (IsRegistered: %d)"),
				*VoipComp->GetName(), VoipComp->IsRegistered());

			// 1. 먼저 Stop 호출
			VoipComp->Stop();

			// 2. 내부 AudioComponent 강제 파괴
			if (UAudioComponent* AudioComp = VoipComp->GetAudioComponent())
			{
				UE_LOG(LogSY, Log, TEXT("Server cleanup - Inner AudioComponent: %s (IsRegistered: %d)"),
					*AudioComp->GetName(), AudioComp->IsRegistered());
				AudioComp->Stop();
				if (AudioComp->IsRegistered())
				{
					AudioComp->UnregisterComponent();
				}
				AudioComp->DestroyComponent();
			}

			// 3. VoipListenerSynthComponent 강제 파괴
			if (VoipComp->IsRegistered())
			{
				VoipComp->UnregisterComponent();
			}
			VoipComp->DestroyComponent();
		}
	}

	// Server 정리 완료 확인 (이번 폴링에서 컴포넌트를 찾지 못하면 완료)
	if (!bServerVoipExists)
	{
		UE_LOG(LogSY, Log, TEXT("✅ Server voice cleanup verified! (Attempts: %d)"),
			ServerCleanupPollingAttempts);
		bServerVoiceCleaned = true;

		// Step 4: 최종 상태 확인 시작
		CheckFinalStateBeforeTravel();
		return;
	}

	// 아직 컴포넌트가 있음 - 다음 폴링에서 다시 확인
	ServerCleanupPollingAttempts++;

	if (ServerCleanupPollingAttempts % 10 == 0) // 100ms마다 로그
	{
		UE_LOG(LogSY, Log, TEXT("⏳ Server still cleaning... (Attempt %d/%d, Destroyed: %d)"),
			ServerCleanupPollingAttempts, MaxServerCleanupPollingAttempts, FoundCount);
	}

	// 타임아웃 체크
	if (ServerCleanupPollingAttempts >= MaxServerCleanupPollingAttempts)
	{
		UE_LOG(LogSY, Warning, TEXT("⚠️ Server voice cleanup timeout! Proceeding anyway. (Destroyed: %d)"),
			FoundCount);
		bServerVoiceCleaned = true;
		CheckFinalStateBeforeTravel();
		return;
	}

	// 다시 체크
	FTimerHandle RetryTimer;
	World->GetTimerManager().SetTimer(RetryTimer,
		this, &UACAdvancedFriendsGameInstance::CheckServerVoiceCleanup,
		ServerPollingInterval, false);
}

void UACAdvancedFriendsGameInstance::CheckFinalStateBeforeTravel()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogSY, Error, TEXT("World is null in final check"));
		DoServerTravel();
		return;
	}

	// ★ Voice 패킷 버퍼 계속 비우기 - 지연된 패킷이 새 컴포넌트를 생성하는 것 방지
	IOnlineVoicePtr Voice = Online::GetVoiceInterface();
	if (Voice.IsValid())
	{
		Voice->ClearVoicePackets();
	}

	// 최종 검증: VoipListenerSynthComponent 및 내부 AudioComponent 직접 확인 (Transient 패키지 포함)
	// DestroyComponent()로 강제 파괴
	bool bVoipComponentsExist = false;
	int32 FoundCount = 0;

	for (TObjectIterator<UVoipListenerSynthComponent> It; It; ++It)
	{
		UVoipListenerSynthComponent* VoipComp = *It;
		if (IsValid(VoipComp))
		{
			bVoipComponentsExist = true;
			FoundCount++;

			UE_LOG(LogSY, Log, TEXT("Final cleanup - Found VoipComp: %s (IsRegistered: %d)"),
				*VoipComp->GetName(), VoipComp->IsRegistered());

			// 1. 먼저 Stop 호출
			VoipComp->Stop();

			// 2. 내부 AudioComponent 강제 파괴
			if (UAudioComponent* AudioComp = VoipComp->GetAudioComponent())
			{
				UE_LOG(LogSY, Log, TEXT("Final cleanup - Inner AudioComponent: %s (IsRegistered: %d)"),
					*AudioComp->GetName(), AudioComp->IsRegistered());
				AudioComp->Stop();
				if (AudioComp->IsRegistered())
				{
					AudioComp->UnregisterComponent();
				}
				AudioComp->DestroyComponent();
			}

			// 3. VoipListenerSynthComponent 강제 파괴
			if (VoipComp->IsRegistered())
			{
				VoipComp->UnregisterComponent();
			}
			VoipComp->DestroyComponent();
		}
	}

	FinalCheckPollingAttempts++;

	// 컴포넌트가 발견되었으면 연속 카운트 리셋
	if (bVoipComponentsExist)
	{
		ConsecutiveCleanPolls = 0;
		UE_LOG(LogSY, Log, TEXT("⏳ Final check - Destroyed %d components (Attempt %d/%d)"),
			FoundCount, FinalCheckPollingAttempts, MaxFinalCheckPollingAttempts);
	}
	else
	{
		// 컴포넌트가 없으면 연속 카운트 증가
		ConsecutiveCleanPolls++;

		// 연속으로 N번 깨끗하면 완료
		if (ConsecutiveCleanPolls >= RequiredCleanPolls)
		{
			UE_LOG(LogSY, Log, TEXT("✅ Final check passed! (Attempts: %d, Clean polls: %d)"),
				FinalCheckPollingAttempts, ConsecutiveCleanPolls);
			DoServerTravel();
			return;
		}

		if (ConsecutiveCleanPolls % 5 == 0) // 50ms마다 로그
		{
			UE_LOG(LogSY, Log, TEXT("⏳ Final check - Clean poll %d/%d (Attempt %d/%d)"),
				ConsecutiveCleanPolls, RequiredCleanPolls, FinalCheckPollingAttempts, MaxFinalCheckPollingAttempts);
		}
	}

	// 타임아웃 체크
	if (FinalCheckPollingAttempts >= MaxFinalCheckPollingAttempts)
	{
		UE_LOG(LogSY, Warning, TEXT("⚠️ Final check timeout! (Clean polls: %d/%d)"),
			ConsecutiveCleanPolls, RequiredCleanPolls);
		DoServerTravel();
		return;
	}

	// 다시 체크
	FTimerHandle RetryTimer;
	World->GetTimerManager().SetTimer(RetryTimer,
		this, &UACAdvancedFriendsGameInstance::CheckFinalStateBeforeTravel,
		ServerPollingInterval, false);
}

void UACAdvancedFriendsGameInstance::TryStartVoice()
{
	UE_LOG(LogSY, Log, TEXT("Try Start Voice"));

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

	// 보이스 연결 (중복 호출되어도 OnlineSubsystem이 처리)
	Voice->RegisterLocalTalker(0);
	Voice->StartNetworkedVoice(0);
	UE_LOG(LogSY, Log, TEXT("Voice initialized"));
}

void UACAdvancedFriendsGameInstance::OnClientVoiceCleanupFinished()
{
	UE_LOG(LogSY, Log, TEXT("OnClientVoiceCleanupFinished!!"));
	++NumClientsReady;

	// 모든 Client가 정리를 완료했는가?
	if (NumClientsReady >= GetWorld()->GetNumPlayerControllers())
	{
		UE_LOG(LogSY, Log, TEXT("=== ✅ All clients finished cleanup ==="));

		// Server Voice는 이미 UpdateMap에서 먼저 정리했으므로 바로 최종 확인으로 이동
		if (GetWorld()->GetNetMode() != NM_Client)
		{
			// 최종 VoipListenerSynthComponent 확인 시작
			ServerCleanupPollingAttempts = 0;
			FinalCheckPollingAttempts = 0;
			ConsecutiveCleanPolls = 0;
			bServerVoiceCleaned = true;
			CheckFinalStateBeforeTravel();
		}
		else
		{
			// Pure Client는 Server 정리 스킵
			bServerVoiceCleaned = true;
			CheckFinalStateBeforeTravel();
		}
	}
}

void UACAdvancedFriendsGameInstance::DoServerTravel()
{
	UE_LOG(LogSY, Log, TEXT("DoServerTravel!!"));

	UWorld* World = GetWorld();
	if (World)
	{
		// ★ Voice 패킷 버퍼 한 번 더 비우기 - 지연된 패킷이 새 컴포넌트를 생성하는 것 방지
		IOnlineVoicePtr Voice = Online::GetVoiceInterface();
		if (Voice.IsValid())
		{
			Voice->ClearVoicePackets();
		}

		// 최종 안전 체크: VoipListenerSynthComponent 및 내부 AudioComponent 강제 파괴 (Transient 패키지 포함)
		int32 ForcedRemovalCount = 0;

		for (TObjectIterator<UVoipListenerSynthComponent> It; It; ++It)
		{
			UVoipListenerSynthComponent* VoipComp = *It;
			if (IsValid(VoipComp))
			{
				ForcedRemovalCount++;

				UE_LOG(LogSY, Warning, TEXT("⚠️ DoServerTravel - Found VoipComp: %s (IsRegistered: %d)"),
					*VoipComp->GetName(), VoipComp->IsRegistered());

				// 1. 먼저 Stop 호출
				VoipComp->Stop();

				// 2. 내부 AudioComponent 강제 파괴
				if (UAudioComponent* AudioComp = VoipComp->GetAudioComponent())
				{
					UE_LOG(LogSY, Warning, TEXT("⚠️ DoServerTravel - Inner AudioComponent: %s (IsRegistered: %d)"),
						*AudioComp->GetName(), AudioComp->IsRegistered());
					AudioComp->Stop();
					if (AudioComp->IsRegistered())
					{
						AudioComp->UnregisterComponent();
					}
					AudioComp->DestroyComponent();
				}

				// 3. VoipListenerSynthComponent 강제 파괴
				if (VoipComp->IsRegistered())
				{
					VoipComp->UnregisterComponent();
				}
				VoipComp->DestroyComponent();
			}
		}

		if (ForcedRemovalCount > 0)
		{
			UE_LOG(LogSY, Warning, TEXT("🔧 DoServerTravel - Force destroyed %d VoIP components, retrying final check..."), ForcedRemovalCount);

			// Audio Flush
			if (FAudioDeviceHandle AudioDeviceHandle = World->GetAudioDevice())
			{
				AudioDeviceHandle->Flush(World);
			}

			// ★ 컴포넌트가 발견되면 다시 최종 확인으로 돌아감
			// 지연된 Voice 패킷으로 인해 새로 생성될 수 있으므로 다시 폴링
			DoServerTravelRetryCount++;
			if (DoServerTravelRetryCount < MaxDoServerTravelRetry)
			{
				FTimerHandle RetryTimer;
				World->GetTimerManager().SetTimer(RetryTimer,
					this, &UACAdvancedFriendsGameInstance::DoServerTravel,
					ServerPollingInterval, false);
				return;
			}
			else
			{
				UE_LOG(LogSY, Warning, TEXT("⚠️ DoServerTravel - Max retry reached, proceeding anyway"));
			}
		}
		else
		{
			UE_LOG(LogSY, Log, TEXT("✅ No lingering VoIP components - safe to travel"));
		}
	}

	// 재시도 카운터 리셋
	DoServerTravelRetryCount = 0;

	switch (CurrentMapType)
	{
	case EMapType::Lobby:
		GetWorld()->ServerTravel("LobbyMap", true);
		break;
	case EMapType::Game:
		GetWorld()->ServerTravel("DemoMap", true);
		break;
	}
}
