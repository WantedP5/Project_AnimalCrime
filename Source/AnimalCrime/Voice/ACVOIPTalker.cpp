
#include "Voice/ACVOIPTalker.h"
#include "Components/AudioComponent.h"

void UACVOIPTalker::OnTalkingBegin(UAudioComponent* AudioComponent)
{
	Super::OnTalkingBegin(AudioComponent);

	CachedAudioComponent = AudioComponent;
	if (AudioComponent == nullptr)
	{
		return;
	}
	AudioComponent->SetVolumeMultiplier(VolumeMultiplier);
}

void UACVOIPTalker::SetVOIPVolume(float NewVolumeMultiplier)
{
	VolumeMultiplier = NewVolumeMultiplier;
	if (CachedAudioComponent == nullptr)
	{
		return;
	}
	CachedAudioComponent->SetVolumeMultiplier(VolumeMultiplier);
}
