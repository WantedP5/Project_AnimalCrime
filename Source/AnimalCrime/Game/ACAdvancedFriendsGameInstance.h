
#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "ACAdvancedFriendsGameInstance.generated.h"

/**

    @class   UACAdvancedFriendsGameInstance
    @brief   
    @details ~

**/
UCLASS()
class ANIMALCRIME_API UACAdvancedFriendsGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()
	
public:
    /** 실제 세션 생성은 BP에서 구현 */
    UFUNCTION(BlueprintImplementableEvent, Category = "Session")
    void CreateSession();
//protected:
//    virtual void Init() override;
//
//    /**
//        @brief 세션을 생성하는 함수
//    **/
//    UFUNCTION()
//    void CreateSession();
//
//private:
    /**
        @brief 세션 생성 성공 콜백 함수
    **/
    //UFUNCTION()
    //void OnCreateSessionSuccess();

    ///**
    //    @brief 세션 생성 실패 콜백 함수
    //**/
    //UFUNCTION()
    //void OnCreateSessionFailure();

    //void OnSessionInviteReceived(const FUniqueNetId& UserId, const FUniqueNetId& FromId, const FString& AppId, const FOnlineSessionSearchResult& InviteResult);

    /**
        @brief 현재 참가중인 세션을 종료하고 새로운 세션으로 참가하는 함수
        @param SessionToJoin - 새로 참가할 대상 세션의 검색 결과
    **/
    //void DestroyCurrentSessionAndJoin(const FBlueprintSessionResult& SessionToJoin);
};

