
#include "ACPlayerControllerBase.h"
#include "ACAdvancedFriendsGameInstance.h"
#include "AudioDevice.h"
#include "AnimalCrime.h"

void AACPlayerControllerBase::Client_CleanupVoiceBeforeTravel_Implementation()
{
	// Voice 인터페이스 정리 (패킷 송수신 중지)
	IOnlineVoicePtr Voice = Online::GetVoiceInterface();
	if (Voice.IsValid())
	{
		Voice->ClearVoicePackets();
		Voice->RemoveAllRemoteTalkers();
		Voice->StopNetworkedVoice(0);
		Voice->UnregisterLocalTalker(0);
	}

	AC_LOG(LogSY, Log, TEXT("=== Client Voice 정리 완료 ==="));
}
