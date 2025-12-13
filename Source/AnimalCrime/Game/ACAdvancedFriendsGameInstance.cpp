
#include "ACAdvancedFriendsGameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "AdvancedSessionsLibrary.h"
#include "CreateSessionCallbackProxyAdvanced.h"

#include "AnimalCrime.h"

//void UACAdvancedFriendsGameInstance::Init()
//{
//    Super::Init();
//}
//
//void UACAdvancedFriendsGameInstance::CreateSession()
//{
//    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
//    if (PC == nullptr)
//    {
//        return;
//    }
//    UCreateSessionCallbackProxyAdvanced* Proxy =
//        UCreateSessionCallbackProxyAdvanced::CreateAdvancedSession(
//            this,
//            TArray<FSessionPropertyKeyPair>(),
//            PC,
//            100,        // PublicConnections
//            0,          // PrivateConnections
//            false,      // bUseLAN
//            true,       // bAllowInvites
//            false,      // bIsDedicatedServer
//            true,       // bUseLobbiesIfAvailable
//            true,       // bAllowJoinViaPresence
//            false,      // bAllowJoinViaPresenceFriendsOnly
//            false,      // bAntiCheatProtected
//            false,      // bUsesStats
//            true,       // bShouldAdvertise
//            false,      // bUseLobbiesVoiceChatIfAvailable
//            true        // bStartAfterCreate
//        );
//
//    if (Proxy == nullptr)
//    {
//        return;
//    }
//
//    Proxy->OnSuccess.AddDynamic(this, &UACAdvancedFriendsGameInstance::OnCreateSessionSuccess);
//    Proxy->OnFailure.AddDynamic(this, &UACAdvancedFriendsGameInstance::OnCreateSessionFailure);
//
//
//}
//
//void UACAdvancedFriendsGameInstance::OnCreateSessionSuccess()
//{
//    UE_LOG(LogTemp, Log, TEXT("Session created successfully"));
//
//    const FString Options = TEXT("listen");
//
//    UGameplayStatics::OpenLevelBySoftObjectPtr(
//        this,
//        TSoftObjectPtr<UWorld>(
//            FSoftObjectPath(TEXT("/Game/Project/Maps/LobbyLevel.LobbyLevel"))
//        ),
//        true,
//        Options
//    );
//}
//
//void UACAdvancedFriendsGameInstance::OnCreateSessionFailure()
//{
//    UE_LOG(LogTemp, Error, TEXT("Failed to create session"));
//}

//void UACAdvancedFriendsGameInstance::OnSessionInviteAccepted_Implementation(int32 LocalPlayerNum, FBPUniqueNetId PersonInvited, const FBlueprintSessionResult& SessionToJoin)
//{
//    UE_LOG(LogSY, Log, TEXT("Session invite accepted"));
//
//    DestroyCurrentSessionAndJoin(SessionToJoin);
//}

//void UACAdvancedFriendsGameInstance::DestroyCurrentSessionAndJoin(const FBlueprintSessionResult& SessionToJoin)
//{
//    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
//    if (!Subsystem) return;
//
//    IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
//    if (!SessionInterface.IsValid()) return;
//
//    FOnDestroySessionCompleteDelegate DestroyDelegate;
//    DestroyDelegate = FOnDestroySessionCompleteDelegate::CreateLambda(
//        [this, SessionToJoin](FName SessionName, bool bWasSuccessful)
//        {
//            UE_LOG(LogTemp, Log, TEXT("DestroySession finished: %d"), bWasSuccessful);
//
//            IOnlineSubsystem* SubsystemInner = IOnlineSubsystem::Get();
//            if (!SubsystemInner) return;
//
//            IOnlineSessionPtr SessionInterfaceInner =
//                SubsystemInner->GetSessionInterface();
//
//            if (!SessionInterfaceInner.IsValid()) return;
//
//            APlayerController* PC =
//                UGameplayStatics::GetPlayerController(this, 0);
//
//            if (!PC) return;
//
//            SessionInterfaceInner->JoinSession(
//                *PC->GetLocalPlayer()->GetPreferredUniqueNetId(),
//                NAME_GameSession,
//                SessionToJoin.OnlineResult
//            );
//        }
//    );
//
//    SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroyDelegate);
//
//    // 세션이 없을 수도 있으니 그냥 호출
//    SessionInterface->DestroySession(NAME_GameSession);
//}
