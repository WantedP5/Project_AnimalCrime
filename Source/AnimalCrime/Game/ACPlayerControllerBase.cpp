
#include "ACPlayerControllerBase.h"
#include "ACAdvancedFriendsGameInstance.h"
#include "AudioDevice.h"
#include "AnimalCrime.h"
#include "Character/ACCharacter.h"
#include "VoipListenerSynthComponent.h"
#include "GameFramework/GameStateBase.h"
#include "RenderingThread.h"

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

	// ★ 클라이언트 자신의 오디오 장치 중단
	if (FAudioDeviceHandle AudioDeviceHandle = World->GetAudioDevice())
	{
		AudioDeviceHandle->SuspendContext();
		AC_LOG(LogSY, Log, TEXT("Client Audio Device Suspended"));
	}

	// 각 플레이어 캐릭터의 VOIPTalker 정리
	if (AGameStateBase* GameState = World->GetGameState())
	{
		int32 CleanedCount = 0;
		for (APlayerState* PS : GameState->PlayerArray)
		{
			// ★ Pawn으로 받아서 캐스팅
			APawn* ACPawn = IsValid(PS) ? PS->GetPawn() : nullptr;
			if (ACPawn == nullptr)
			{
				continue;
			}

			AACCharacter* ACChar = Cast<AACCharacter>(ACPawn);
			if (ACChar == nullptr)
			{
				continue;
			}

			if (ACChar->VOIPTalker != nullptr)
			{
				ACChar->CleanupVOIPTalker();
				CleanedCount++;
			}
		}
		AC_LOG(LogSY, Log, TEXT("Cleaned %d VOIPTalkers"), CleanedCount);
	}
	
	IOnlineVoicePtr Voice = Online::GetVoiceInterface();
	if (Voice.IsValid() == true)
	{
		// ★ Voice 패킷 버퍼 먼저 비우기 - 버퍼에 남아있는 패킷이 처리되어 새 컴포넌트가 생성되는 것 방지
		Voice->ClearVoicePackets();
		Voice->RemoveAllRemoteTalkers();
		Voice->StopNetworkedVoice(0);
		Voice->UnregisterLocalTalker(0);
		AC_LOG(LogSY, Log, TEXT("Voice cleanup completed"));
	}
	else
	{
		AC_LOG(LogSY, Warning, TEXT("Voice is fail"));
	}

	// VoipListenerSynthComponent 직접 정리 (Transient 패키지 포함)
	// 내부 AudioComponent도 함께 정리해야 함 (UE-146893, UE-169798 버그 대응)
	// IsRegistered() 여부와 관계없이 모두 DestroyComponent()로 강제 파괴
	int32 TotalVoipCount = 0;
	for (TObjectIterator<UVoipListenerSynthComponent> It; It; ++It)
	{
		UVoipListenerSynthComponent* VoipComp = *It;
		if (IsValid(VoipComp))
		{
			TotalVoipCount++;
			AC_LOG(LogSY, Log, TEXT("Client cleanup - Found VoipComp: %s (IsRegistered: %d)"),
				*VoipComp->GetName(), VoipComp->IsRegistered());

			// 1. 먼저 Stop 호출
			VoipComp->Stop();

			// 2. 내부 AudioComponent 강제 파괴
			if (UAudioComponent* AudioComp = VoipComp->GetAudioComponent())
			{
				AC_LOG(LogSY, Log, TEXT("Client cleanup - Inner AudioComponent: %s (IsRegistered: %d)"),
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
	AC_LOG(LogSY, Log, TEXT("Client cleanup - Total VoipComponents destroyed: %d"), TotalVoipCount);

	// AudioDevice 정리
	if (FAudioDeviceHandle AudioDeviceHandle = World->GetAudioDevice())
	{
		// StopAllSounds는 게임 스레드에서 안전
		AudioDeviceHandle->StopAllSounds(true);
		// Flush로 오디오 처리 대기
		AudioDeviceHandle->Flush(World);
	}

	FAudioCommandFence Fence;
	Fence.BeginFence();
	Fence.Wait();

	//Server_NotifyVoiceCleaned();

	// 약간의 지연을 준 뒤 Polling 시작
	ClientPollingAttempts = 0;
	ConsecutiveCleanPolls = 0;
	FTimerHandle StartPollTimer;
	GetWorldTimerManager().SetTimer(
		StartPollTimer,
		this,
		&AACPlayerControllerBase::CheckClientVoiceCleanupComplete,
		1.0f,
		false
	);

	AC_LOG(LogSY, Log, TEXT("Server_NotifyVoiceCleaned"));
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

	// ★ Voice 패킷 버퍼 계속 비우기 - 지연된 패킷이 새 컴포넌트를 생성하는 것 방지
	IOnlineVoicePtr Voice = Online::GetVoiceInterface();
	if (Voice.IsValid())
	{
		Voice->ClearVoicePackets();
	}

	// VoipListenerSynthComponent 및 내부 AudioComponent 직접 확인 (Transient 패키지 포함)
	// 새로 생성되는 컴포넌트도 계속 잡아서 DestroyComponent()로 강제 파괴
	bool bVoipComponentsExist = false;
	int32 FoundCount = 0;

	for (TObjectIterator<UVoipListenerSynthComponent> It; It; ++It)
	{
		UVoipListenerSynthComponent* VoipComp = *It;
		if (IsValid(VoipComp))
		{
			bVoipComponentsExist = true;
			FoundCount++;

			AC_LOG(LogSY, Log, TEXT("Polling - Found VoipComp: %s (IsRegistered: %d)"),
				*VoipComp->GetName(), VoipComp->IsRegistered());

			// 1. 먼저 Stop 호출
			VoipComp->Stop();

			// 2. 내부 AudioComponent 강제 파괴
			if (UAudioComponent* AudioComp = VoipComp->GetAudioComponent())
			{
				AC_LOG(LogSY, Log, TEXT("Polling - Inner AudioComponent: %s (IsRegistered: %d)"),
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

	ClientPollingAttempts++;

	// 컴포넌트가 발견되었으면 연속 카운트 리셋
	if (bVoipComponentsExist)
	{
		ConsecutiveCleanPolls = 0;
		AC_LOG(LogSY, Log, TEXT("⏳ Polling - Destroyed %d components (Attempt %d/%d)"),
			FoundCount, ClientPollingAttempts, MaxClientPollingAttempts);
	}
	else
	{
		// 컴포넌트가 없으면 연속 카운트 증가
		ConsecutiveCleanPolls++;

		// 연속으로 N번 깨끗하면 완료
		if (ConsecutiveCleanPolls >= RequiredCleanPolls)
		{
			AC_LOG(LogSY, Log, TEXT("✅ Client voice cleanup verified! (Attempts: %d, Clean polls: %d)"),
				ClientPollingAttempts, ConsecutiveCleanPolls);
			Server_NotifyVoiceCleaned();
			return;
		}

		AC_LOG(LogSY, Log, TEXT("⏳ Polling - Clean poll %d/%d (Attempt %d/%d)"),
			ConsecutiveCleanPolls, RequiredCleanPolls, ClientPollingAttempts, MaxClientPollingAttempts);
	}

	// 타임아웃 체크
	if (ClientPollingAttempts >= MaxClientPollingAttempts)
	{
		AC_LOG(LogSY, Warning, TEXT("⚠️ Client voice cleanup timeout! (Clean polls: %d/%d)"),
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
