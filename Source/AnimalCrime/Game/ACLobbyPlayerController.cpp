
#include "ACLobbyPlayerController.h"
#include "UI/GameStart/ACLobbyScreen.h"
#include "Blueprint/UserWidget.h"
#include "ACLobbyGameMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInput/Public/InputMappingContext.h"
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
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContextRef(TEXT("/Game/Project/Input/IMC_Shoulder.IMC_Shoulder"));
	if (DefaultMappingContextRef.Succeeded())
	{
		DefaultMappingContext = DefaultMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> SettingsMappingContextRef(TEXT("/Game/Project/Input/IMC_Settings.IMC_Settings"));
	if (SettingsMappingContextRef.Succeeded())
	{
		SettingsMappingContext = SettingsMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Game/Project/Input/Actions/IA_Move.IA_Move"));
	if (MoveActionRef.Succeeded())
	{
		MoveAction = MoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Game/Project/Input/Actions/IA_Look.IA_Look"));
	if (LookActionRef.Succeeded())
	{
		LookAction = LookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Game/Project/Input/Actions/IA_Jump.IA_Jump"));
	if (JumpActionRef.Succeeded())
	{
		JumpAction = JumpActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InteractActionRef(TEXT("/Game/Project/Input/Actions/IA_Interact.IA_Interact"));
	if (InteractActionRef.Succeeded())
	{
		InteractAction = InteractActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ItemDropActionRef(TEXT("/Game/Project/Input/Actions/IA_ItemDrop.IA_ItemDrop"));
	if (ItemDropActionRef.Succeeded())
	{
		ItemDropAction = ItemDropActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MeleeActionRef(TEXT("/Game/Project/Input/Actions/IA_Attack.IA_Attack"));
	if (MeleeActionRef.Succeeded())
	{
		MeleeAction = MeleeActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SettingsCloseActionRef(TEXT("/Game/Project/Input/Actions/IA_SettingsClose.IA_SettingsClose"));
	if (SettingsCloseActionRef.Succeeded())
	{
		SettingsCloseAction = SettingsCloseActionRef.Object;
	}

    static ConstructorHelpers::FObjectFinder<UInputAction> SteamFriendListActionRef(TEXT("/Game/Project/Input/Actions/IA_SteamFriendList.IA_SteamFriendList"));
    if (SteamFriendListActionRef.Succeeded())
    {
        SteamFriendListAction = SteamFriendListActionRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> ReadyActionRef(TEXT("/Game/Project/Input/Actions/IA_Ready.IA_Ready"));
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

void AACLobbyPlayerController::ServerStartGame_Implementation()
{
	AGameModeBase* GMBase = GetWorld()->GetAuthGameMode();
	if (GMBase == nullptr)
	{
		return;
	}

	AACLobbyGameMode* GM = Cast< AACLobbyGameMode>(GMBase);
	if (GM == nullptr)
	{
		return;
	}

	GM->StartGamePlay();

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

	// Enhanced Input Subsystem에 기본 매핑 컨텍스트 추가
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

    // 입력 액션 바인딩
	if (MoveAction)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AACLobbyPlayerController::HandleMove);
	}
	if (LookAction)
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AACLobbyPlayerController::HandleLook);
	}
	if (JumpAction)
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AACLobbyPlayerController::HandleJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AACLobbyPlayerController::HandleStopJumping);
	}
	if (InteractAction)
	{
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AACLobbyPlayerController::HandleInteract);
	}
	if (ItemDropAction)
	{
		EnhancedInputComponent->BindAction(ItemDropAction, ETriggerEvent::Started, this, &AACLobbyPlayerController::HandleItemDrop);
	}
	if (MeleeAction)
	{
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Started, this, &AACLobbyPlayerController::HandleAttack);
	}
	if (SettingsCloseAction)
	{
		EnhancedInputComponent->BindAction(SettingsCloseAction, ETriggerEvent::Started, this, &AACLobbyPlayerController::HandleSettingsClose);
	}
    if (SteamFriendListAction)
    {
        EnhancedInputComponent->BindAction(SteamFriendListAction, ETriggerEvent::Started, this, &AACLobbyPlayerController::HandleSteamFriendList);
    }
    if (ReadyAction)
    {
        EnhancedInputComponent->BindAction(ReadyAction, ETriggerEvent::Started, this, &AACLobbyPlayerController::HandleGameReady);
    }
}

// ===== 입력 핸들러 구현 =====    
void AACLobbyPlayerController::HandleMove(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter)
	{
		ControlledCharacter->Move(Value);
	}
}

void AACLobbyPlayerController::HandleLook(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter)
	{
		ControlledCharacter->Look(Value);
	}
}

void AACLobbyPlayerController::HandleJump(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter)
	{
		ControlledCharacter->Jump();
	}
}

void AACLobbyPlayerController::HandleStopJumping(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter)
	{
		ControlledCharacter->StopJumping();
	}
}

void AACLobbyPlayerController::HandleInteract(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter)
	{
		ControlledCharacter->Interact(Value);
	}
}

void AACLobbyPlayerController::HandleItemDrop(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter)
	{
		ControlledCharacter->ItemDrop(Value);
	}
}

void AACLobbyPlayerController::HandleAttack(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter)
	{
		ControlledCharacter->Attack();
	}
}

void AACLobbyPlayerController::HandleSettingsClose(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter)
	{
		ControlledCharacter->SettingsClose(Value);
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

void AACLobbyPlayerController::ChangeInputMode(EInputMode NewMode)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// 모든 매핑 컨텍스트 제거
		Subsystem->ClearAllMappings();

		// 모드에 따라 적절한 매핑 컨텍스트 추가
		if (NewMode == EInputMode::Sholder && DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		else if (NewMode == EInputMode::Settings && SettingsMappingContext)
		{
			Subsystem->AddMappingContext(SettingsMappingContext, 0);
		}
	}
}