
#include "ACAdvancedFriendsGameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "AdvancedSessionsLibrary.h"
#include "CreateSessionCallbackProxyAdvanced.h"
#include "Engine/NetDriver.h"
#include "Engine/Engine.h"

#include "AnimalCrime.h"

#pragma region 엔진 제공 함수
void UACAdvancedFriendsGameInstance::Init()
{
    Super::Init();
	
    //네트워크 끊김 처리
    GetEngine()->OnNetworkFailure().AddUObject(this, &UACAdvancedFriendsGameInstance::HandleNetworkFailure);
    GetEngine()->OnTravelFailure().AddUObject(this, &UACAdvancedFriendsGameInstance::HandleTravelFailure);
    
    
    // 맵 관련 초기화
    CurrentMapType = EMapType::None;
	
    LobbyMapName = TEXT("LobbyMap");
    GameMapName = TEXT("HenaMap");
	
    GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([]()
    {
        UE_LOG(LogHY, Warning, TEXT("여긴 어딜까?"));
    }),10, false);
    
    UE_LOG(LogTemp, Warning, TEXT("UACMainGameInstance::Init"));
}

void UACAdvancedFriendsGameInstance::Shutdown()
{
    Super::Shutdown();
	
    UE_LOG(LogTemp, Warning, TEXT("UACMainGameInstance::Shutdown"));
}

void UACAdvancedFriendsGameInstance::OnStart()
{
    Super::OnStart();
	
    UE_LOG(LogTemp, Warning, TEXT("UACMainGameInstance::OnStart"));
}
#pragma endregion

#pragma region Map Level
void UACAdvancedFriendsGameInstance::LoadLobbyMap()
{
    UpdateMap(EMapType::Lobby);
}

void UACAdvancedFriendsGameInstance::LoadGameMap()
{
    UpdateMap(EMapType::Game);
}

EMapType UACAdvancedFriendsGameInstance::GetMapType() const
{
    return CurrentMapType;
}

void UACAdvancedFriendsGameInstance::UpdateMap(const EMapType InMapType)
{
    if (CurrentMapType == InMapType)
    {
        UE_LOG(LogTemp, Error, TEXT("CurrentMapType == InMapType"));
        return ;
    }
    UWorld* World = GetWorld();
    switch (InMapType)
    {
    case EMapType::Lobby:
        {
            // UE_LOG(LogTemp, Error, TEXT("엥 말도 안돼"));
            // if (GEngine->IsEditor() && World->WorldType == EWorldType::PIE)
            // {
            //     UGameplayStatics::OpenLevel(World, FName("LobbyMap"));
            // }
            // else
            // {
            //     //GetWorld()->ServerTravel(LobbyMapName);
            //     World->ServerTravel("/Game/Project/Map/LobbyMap");
            // }
            break;
        }
        
        
    case EMapType::Game:
        {
            // if (World->IsNetMode(NM_Standalone))
            // {
            //     UGameplayStatics::OpenLevel(World, FName("henaMap"));
            // }
            // else
            // {
                World->ServerTravel("/Game/Project/Map/henaMap?listen");
            //}
            // if (GEngine->IsEditor() && World->WorldType == EWorldType::PIE)
            // {
            //     UE_LOG(LogTemp, Error, TEXT("EMapType::Game == EWorldType::PIE"));
            //     UGameplayStatics::OpenLevel(World, FName("henaMap"));
            // }
            // else
            // {
            //     //GetWorld()->ServerTravel(LobbyMapName);
            //     UE_LOG(LogTemp, Error, TEXT("EMapType::Game != EWorldType::PIE"));
            //     World->ServerTravel("/Game/Project/Map/henaMap");
            // }
        }
        
        break;
    default:
        break;
    }
	
    // 현재 맵을 변경된 맵으로 Update
    CurrentMapType = InMapType;
}
#pragma endregion

void UACAdvancedFriendsGameInstance::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
    UE_LOG(LogSY, Warning, TEXT("Network Failure: %s"), *ErrorString);

    // 세션 정리
    IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(World);
    if (SessionInterface.IsValid() == true)
    {
        SessionInterface->DestroySession(NAME_GameSession);
    }
}

void UACAdvancedFriendsGameInstance::HandleTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString)
{
    UE_LOG(LogSY, Warning, TEXT("Travel Failure: %s"), *ErrorString);

    // 세션 정리
    IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(World);
    if (SessionInterface.IsValid() == true)
    {
        SessionInterface->DestroySession(NAME_GameSession);
    }
}
