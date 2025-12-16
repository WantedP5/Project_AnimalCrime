
#include "ACLobbyPlayerController.h"
#include "UI/GameStart/ACLobbyScreen.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Character/ACLobbyCharacter.h"
#include "AnimalCrime.h"

AACLobbyPlayerController::AACLobbyPlayerController()
{
    //Lobby 스크린 로드
    static ConstructorHelpers::FClassFinder<UACLobbyScreen> LobbyScreenRef(TEXT("/Game/Project/UI/GameStart/WBP_LobbyScreen.WBP_LobbyScreen_C"));
    if (LobbyScreenRef.Succeeded())
    {
        LobbyScreenClass = LobbyScreenRef.Class;
    }

    //SteamFriendList 로드
    static ConstructorHelpers::FClassFinder<UUserWidget> SteamFriendListRef(TEXT("/Game/Project/UI/GameStart/WBP_FriendList.WBP_FriendList_C"));
    if (SteamFriendListRef.Succeeded())
    {
        SteamFriendListClass = SteamFriendListRef.Class;
    }

    // ===== 입력 관련 로드 =====
    static ConstructorHelpers::FObjectFinder<UInputAction> SteamFriendListActionRef(TEXT("/Game/Project/Input/Actions/IA_SteamFriendList.IA_SteamFriendList"));
    if (SteamFriendListActionRef.Succeeded())
    {
        SteamFriendListAction = SteamFriendListActionRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> ReadyActionRef(TEXT("/Game/Project/Input/Actions/IA_GameReady.IA_GameReady"));
    if (ReadyActionRef.Succeeded())
    {
        ReadyAction = ReadyActionRef.Object;
    }
}

void AACLobbyPlayerController::SteamFriendListToggle(bool bVisible)
{
    if (SteamFriendList == nullptr)
    {
        AC_LOG(LogSY, Warning, TEXT("SteamFriendList is nullptr"));
        return;
    }

    if (bVisible == true)
    {
        AC_LOG(LogSY, Log, TEXT("open"));
        SteamFriendList->AddToViewport();

        // 마우스 커서 켜기
        bShowMouseCursor = true;

        // UI와 게임 입력 둘 다 받기
        FInputModeGameAndUI InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        SetInputMode(InputMode);
    }
    else
    {
        AC_LOG(LogSY, Log, TEXT("close"));
        SteamFriendList->RemoveFromParent();
        // 마우스 커서 끄기
        bShowMouseCursor = false;
        bEnableClickEvents = false;
        bEnableMouseOverEvents = false;
        SetInputMode(FInputModeGameOnly());
    }

}

void AACLobbyPlayerController::UpdatePlayerReadyText(int32 AllplayerNum)
{
    LobbyScreen->UpdatePlayerReadyText(0, AllplayerNum);
}

void AACLobbyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 로컬 플레이어만 UI 생성
    if (!IsLocalController())
    {
        return;
    }

    LobbyScreen = CreateWidget<UACLobbyScreen>(this, LobbyScreenClass);
    if (LobbyScreen == nullptr)
    {
        return;
    }
    LobbyScreen->AddToViewport();

    // 마우스 커서 숨김
    bShowMouseCursor = false;

    // 클릭 끔
    bEnableClickEvents = false;
    bEnableMouseOverEvents = false;

    // 게임 입력만 받기
    SetInputMode(FInputModeGameOnly());

    // SteamFriendList 생성
    SteamFriendList = CreateWidget<UUserWidget>(this, SteamFriendListClass);
}

void AACLobbyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
    if (EnhancedInputComponent == nullptr)
    {
        AC_LOG(LogTemp, Error, TEXT("EnhancedInputComponent is nullptr"));
        return;
    }

    // 입력 액션 바인딩
    if (SteamFriendListAction)
    {
        EnhancedInputComponent->BindAction(SteamFriendListAction, ETriggerEvent::Started, this, &AACLobbyPlayerController::HandleSteamFriendList);
    }
    if (ReadyAction)
    {
        EnhancedInputComponent->BindAction(ReadyAction, ETriggerEvent::Started, this, &AACLobbyPlayerController::HandleGameReady);
    }
}

void AACLobbyPlayerController::HandleSteamFriendList(const FInputActionValue& Value)
{
    AACLobbyCharacter* LobbyChar = GetPawn<AACLobbyCharacter>();
    if (LobbyChar)
    {
        LobbyChar->SetSteamFriendsList(Value);
    }
}

void AACLobbyPlayerController::HandleGameReady(const FInputActionValue& Value)
{
    AACLobbyCharacter* LobbyChar = GetPawn<AACLobbyCharacter>();
    if (LobbyChar)
    {
        LobbyChar->GameReady(Value);
    }
}