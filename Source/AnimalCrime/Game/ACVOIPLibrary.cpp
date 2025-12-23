
#include "Game/ACVOIPLibrary.h"
#include "AnimalCrime.h"
void UACVOIPLibrary::ClearVoicePackets(UObject* WorldContextObject)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if (IsValid(World) == false)
	{
		return;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface(World);

	if (VoiceInterface.IsValid() == false)
	{
		UE_LOG(LogSY, Display, TEXT("Clear voice packets couldn't get the voice interface"));
	}

	VoiceInterface->ClearVoicePackets();
}
