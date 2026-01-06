
#include "ACAdvancedFriendsGameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/NetDriver.h"
#include "Engine/Engine.h"
#include "AnimalCrime.h"
#include "Components/AudioComponent.h"
#include "ACPlayerControllerBase.h"
#include "VoipListenerSynthComponent.h"

#pragma region 엔진 제공 함수
void UACAdvancedFriendsGameInstance::Init()
{
	Super::Init();

	// 네트워크 끊김 처리
	GetEngine()->OnNetworkFailure().AddUObject(this, &UACAdvancedFriendsGameInstance::HandleNetworkFailure);
	GetEngine()->OnTravelFailure().AddUObject(this, &UACAdvancedFriendsGameInstance::HandleTravelFailure);

	OnWorldCleanupHandle = FWorldDelegates::OnWorldCleanup.AddUObject(this, &UACAdvancedFriendsGameInstance::OnWorldCleanup);

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
	if (OnWorldCleanupHandle.IsValid())
	{
		FWorldDelegates::OnWorldCleanup.Remove(OnWorldCleanupHandle);
		OnWorldCleanupHandle.Reset();
	}
	Super::Shutdown();
}

void UACAdvancedFriendsGameInstance::OnWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
	UE_LOG(LogSY, Log, TEXT("=== OnWorldCleanup: %s ==="), *World->GetName());

	// VoipListenerSynthComponent 강제 정리
	for (TObjectIterator<UVoipListenerSynthComponent> It; It; ++It)
	{
		UVoipListenerSynthComponent* VoipComp = *It;
		if (IsValid(VoipComp))
		{
			UE_LOG(LogSY, Log, TEXT("OnWorldCleanup - Destroying VoipComp: %s"), *VoipComp->GetName());

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

	UE_LOG(LogSY, Log, TEXT("=== OnWorldCleanup 완료 ==="));
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

	CurrentMapType = InMapType;

	// Server Voice 인터페이스 정리 (패킷 중계 중지)
	if (GetWorld()->GetNetMode() != NM_Client)
	{
		IOnlineVoicePtr Voice = Online::GetVoiceInterface();
		if (Voice.IsValid())
		{
			Voice->ClearVoicePackets();
			Voice->RemoveAllRemoteTalkers();
			Voice->StopNetworkedVoice(0);
			Voice->UnregisterLocalTalker(0);
		}
	}

	// Client들에게 Voice 정리 요청
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AACPlayerControllerBase* PC = Cast<AACPlayerControllerBase>(It->Get());
		if (PC != nullptr)
		{
			PC->Client_CleanupVoiceBeforeTravel();
		}
	}

	DoServerTravel();
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

void UACAdvancedFriendsGameInstance::DoServerTravel()
{
	UE_LOG(LogSY, Log, TEXT("=== DoServerTravel ==="));

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
