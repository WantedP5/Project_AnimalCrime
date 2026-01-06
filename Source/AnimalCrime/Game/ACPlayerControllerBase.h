#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ACPlayerControllerBase.generated.h"

UCLASS()
class ANIMALCRIME_API AACPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Client, Reliable)
	void Client_CleanupVoiceBeforeTravel();
};
