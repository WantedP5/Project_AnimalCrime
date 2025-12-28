
#include "ACPlayerControllerBase.h"
#include "ACAdvancedFriendsGameInstance.h"
#include "AudioDevice.h"

#include "AnimalCrime.h"

void AACPlayerControllerBase::Client_CleanupVoiceBeforeTravel_Implementation()
{
	AC_LOG(LogSY, Log, TEXT("Client_CleanupVoiceBeforeTravel!!"));
	IOnlineVoicePtr Voice = Online::GetVoiceInterface();
	if (Voice == nullptr)
	{
		AC_LOG(LogSY, Warning, TEXT("Voice is nullptr"));
		return;
	}
	Voice->StopNetworkedVoice(0);
	Voice->UnregisterLocalTalker(0);

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
	GI->OnClientVoiceCleanupFinished();
	AC_LOG(LogSY, Log, TEXT("Server_NotifyVoiceCleaned!!"));
}