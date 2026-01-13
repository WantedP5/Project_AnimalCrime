
#pragma once

#include "CoreMinimal.h"
#include "Net/VoiceConfig.h"
#include "ACVOIPTalker.generated.h"

UCLASS()
class ANIMALCRIME_API UACVOIPTalker : public UVOIPTalker
{
	GENERATED_BODY()

protected:
    virtual void OnTalkingBegin(UAudioComponent* AudioComponent) override;

public:
    UFUNCTION(BlueprintCallable, Category = "VOIP")
    void SetVOIPVolume(float NewVolumeMultiplier);
private:
    UPROPERTY()
    TObjectPtr<UAudioComponent> CachedAudioComponent;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VOIP")
    float VolumeMultiplier = 3.0f;

};
