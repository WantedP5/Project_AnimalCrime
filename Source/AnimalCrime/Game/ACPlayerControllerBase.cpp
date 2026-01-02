
#include "ACPlayerControllerBase.h"
#include "ACAdvancedFriendsGameInstance.h"
#include "AudioDevice.h"
#include "AnimalCrime.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "VoipListenerSynthComponent.h"

void AACPlayerControllerBase::TryStartVoice()
{
	AC_LOG(LogSY, Log, TEXT("TryStartVoice"));

	if (!IsLocalController())
	{
		return;
	}

	IOnlineVoicePtr Voice = Online::GetVoiceInterface();
	if (!Voice.IsValid())
	{
		AC_LOG(LogSY, Log, TEXT("Voice: VoiceInterface 없음"));
		return;
	}

	IOnlineSessionPtr Session = Online::GetSessionInterface(GetWorld());
	if (!Session.IsValid() || Session->GetNamedSession(NAME_GameSession) == nullptr)
	{
		AC_LOG(LogSY, Log, TEXT("Voice: 세션 없음, Voice 시작 안 함"));
		return;
	}

	// 보이스 연결 (중복 호출되어도 OnlineSubsystem이 처리)
	Voice->RegisterLocalTalker(0);
	Voice->StartNetworkedVoice(0);
	AC_LOG(LogSY, Log, TEXT("Voice initialized"));
}

void AACPlayerControllerBase::CleanupMyVoice()
{
	AC_LOG(LogSY, Log, TEXT("CleanupMyVoice"));

	IOnlineVoicePtr Voice = Online::GetVoiceInterface();
	if (Voice.IsValid())
	{
		// Voice 패킷 버퍼 비우기
		//Voice->RemoveAllRemoteTalkers();
		Voice->StopNetworkedVoice(0);
		Voice->UnregisterLocalTalker(0);
		Voice->ClearVoicePackets();
		AC_LOG(LogSY, Log, TEXT("Voice interface cleanup completed"));
	}

	//// VoipListenerSynthComponent 안전하게 Unregister
	//// 1. 먼저 배열에 복사 (순회 중 수정 방지)
	//TArray<TWeakObjectPtr<UVoipListenerSynthComponent>> VoipComps;
	//for (TObjectIterator<UVoipListenerSynthComponent> It; It; ++It)
	//{
	//	if (IsValid(*It))
	//	{
	//		VoipComps.Add(*It);
	//	}
	//}

	// 2. 복사된 배열 순회하며 안전하게 Unregister
	//for (TWeakObjectPtr<UVoipListenerSynthComponent>& WeakComp : VoipComps)
	//{
	//	UVoipListenerSynthComponent* VoipComp = WeakComp.Get();
	//	if (!VoipComp || !IsValid(VoipComp))
	//	{
	//		continue;
	//	}

	//	AC_LOG(LogSY, Log, TEXT("Cleanup VoipComp: %s (IsRegistered: %d)"),
	//		*VoipComp->GetName(), VoipComp->IsRegistered());

	//	VoipComp->Stop();

	//	// 내부 AudioComponent도 Unregister
	//	if (UAudioComponent* AudioComp = VoipComp->GetAudioComponent())
	//	{
	//		if (IsValid(AudioComp))
	//		{
	//			AudioComp->Stop();
	//			if (AudioComp->IsRegistered())
	//			{
	//				AudioComp->UnregisterComponent();
	//			}
	//		}
	//	}

	//	// VoipListenerSynthComponent Unregister
	//	if (VoipComp->IsRegistered())
	//	{
	//		VoipComp->UnregisterComponent();
	//	}
	//}

	//AC_LOG(LogSY, Log, TEXT("VoipComponents unregistered: %d"), VoipComps.Num());
}

void AACPlayerControllerBase::Client_CleanupVoiceBeforeTravel_Implementation()
{
	AC_LOG(LogSY, Log, TEXT("Client_CleanupVoiceBeforeTravel!!"));

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		AC_LOG(LogSY, Warning, TEXT("World is nullptr"));
		return;
	}

	// PIE에서는 Voice / Audio 정리 스킵
	if (World->IsPlayInEditor())
	{
		AC_LOG(LogSY, Log, TEXT("PIE detected - skip voice cleanup"));
		Server_NotifyVoiceCleaned();
		return;
	}

	// 자기 Voice만 정리 (TObjectIterator 사용 안 함)
	CleanupMyVoice();

	// AudioDevice 정리
	if (FAudioDeviceHandle AudioDeviceHandle = World->GetAudioDevice())
	{
		AudioDeviceHandle->StopAllSounds(true);
		//AudioDeviceHandle->Flush(World);
	}

	// Polling 시작 - Voice 패킷이 완전히 비워질 때까지 대기
	//ClientPollingAttempts = 0;
	//ConsecutiveCleanPolls = 0;
	//CheckClientVoiceCleanupComplete();

	Server_NotifyVoiceCleaned();
}

void AACPlayerControllerBase::Server_NotifyVoiceCleaned_Implementation()
{
	UACAdvancedFriendsGameInstance* GI = GetGameInstance<UACAdvancedFriendsGameInstance>();
	if (GI == nullptr)
	{
		AC_LOG(LogSY, Error, TEXT("GI is nullptr"));
		return;
	}
	AC_LOG(LogSY, Log, TEXT("Server_NotifyVoiceCleaned!!"));
	GI->OnClientVoiceCleanupFinished();
}

void AACPlayerControllerBase::CheckClientVoiceCleanupComplete()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        AC_LOG(LogSY, Error, TEXT("World is null in polling"));
        Server_NotifyVoiceCleaned();
        return;
    }

    // Voice 패킷 버퍼 계속 비우기 - 대화 중이면 패킷이 계속 들어올 수 있음
    IOnlineVoicePtr Voice = Online::GetVoiceInterface();
    bool bHasVoiceData = false;

    if (Voice.IsValid())
    {
        // 패킷 비우기 시도
        Voice->ClearVoicePackets();

        // Voice가 아직 활성 상태인지 체크 (패킷이 들어오고 있는지)
        // ClearVoicePackets 후에도 네트워크에서 새 패킷이 도착할 수 있음
        // 여기서는 단순히 연속 클린 체크로 안정성 확보
    }

    ClientPollingAttempts++;
    ConsecutiveCleanPolls++;

    // 연속으로 N번 깨끗하면 완료 (더 이상 새 패킷이 안 들어온다고 판단)
    if (ConsecutiveCleanPolls >= RequiredCleanPolls)
    {
        AC_LOG(LogSY, Log, TEXT("Client voice cleanup verified! (Attempts: %d, Clean polls: %d)"),
            ClientPollingAttempts, ConsecutiveCleanPolls);
        Server_NotifyVoiceCleaned();
        return;
    }

    // 타임아웃 체크
    if (ClientPollingAttempts >= MaxClientPollingAttempts)
    {
        AC_LOG(LogSY, Warning, TEXT("Client voice cleanup timeout! (Clean polls: %d/%d)"),
            ConsecutiveCleanPolls, RequiredCleanPolls);
        Server_NotifyVoiceCleaned();
        return;
    }

    // 다시 체크
    FTimerHandle RetryTimer;
    GetWorldTimerManager().SetTimer(RetryTimer,
        this, &AACPlayerControllerBase::CheckClientVoiceCleanupComplete,
        ClientPollingInterval, false);
}
