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

	UFUNCTION(Server, Reliable)
	void Server_NotifyVoiceCleaned();

private:
	void CheckClientVoiceCleanupComplete();

private:
	int32 ClientPollingAttempts = 0;
	int32 ConsecutiveCleanPolls = 0; // 연속으로 컴포넌트가 없었던 횟수
	static constexpr int32 MaxClientPollingAttempts = 100; // 최대 1초 (100 * 10ms)
	static constexpr int32 RequiredCleanPolls = 20; // 연속 20번(200ms) 깨끗해야 완료
	static constexpr float ClientPollingInterval = 0.01f; // 10ms
};
