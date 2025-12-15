
#include "ACAdvancedFriendsGameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "AdvancedSessionsLibrary.h"
#include "CreateSessionCallbackProxyAdvanced.h"
#include "Engine/NetDriver.h"
#include "Engine/Engine.h"

#include "AnimalCrime.h"

void UACAdvancedFriendsGameInstance::Init()
{
	Super::Init();

	//네트워크 끊김 처리
	GetEngine()->OnNetworkFailure().AddUObject(this, &UACAdvancedFriendsGameInstance::HandleNetworkFailure);
	GetEngine()->OnTravelFailure().AddUObject(this, &UACAdvancedFriendsGameInstance::HandleTravelFailure);
}

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