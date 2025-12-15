
#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "ACAdvancedFriendsGameInstance.generated.h"

UCLASS()
class ANIMALCRIME_API UACAdvancedFriendsGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()
	
public:
    virtual void Init() override;

public:
    /** 실제 세션 생성은 BP에서 구현 */
    UFUNCTION(BlueprintImplementableEvent, Category = "Session")
    void CreateSession();

protected:

    /**
        @brief 네트워크 실패 시 처리 로직
        @param World       - 어느 월드에서 발생했는지
        @param NetDriver   - 어떤 네트워크 드라이버인지
        @param FailureType - 실패 타입 (연결 끊김, 타임아웃 등)
        @param ErrorString - 에러 메시지
    **/
    UFUNCTION()
    void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

    /**
        @brief 맵 이동 실패 시 처리 로직
        @param World       - 어느 월드에서 발생했는지
        @param FailureType - 실패 타입
        @param ErrorString - 에러 메시지
    **/
    UFUNCTION()
    void HandleTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString);
};

