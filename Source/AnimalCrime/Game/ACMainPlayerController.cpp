
#include "ACMainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "ACTitlePlayerController.h"
#include "UI/Shop/ACShopWidget.h"
#include "AnimalCrime.h"
#include "ACAdvancedFriendsGameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/ACCharacter.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "UI/ACHUDWidget.h"
#include "UI/QuickSlot/ACQuickSlotWidget.h"
#include "UI/CCTV/ACCCTVWidget.h"
#include "UI/Interaction/ACInteractProgressWidget.h"
#include "UI/GameStart/ACRoleScreen.h"
#include "ACPlayerState.h"
#include "ACMainGameState.h"
#include "ACMainGameMode.h"
#include "ACGameRuleManager.h"
#include "EngineUtils.h"
#include "OnlineSubsystem.h"
#include "GameFramework/GameStateBase.h"

AACMainPlayerController::AACMainPlayerController()
{
	//탈출 스크린 로드
	static ConstructorHelpers::FClassFinder<UUserWidget> EscapeScreenRef(TEXT("/Game/Project/UI/WBP_EscapeText.WBP_EscapeText_C"));
	if (EscapeScreenRef.Succeeded())
	{
		EscapeScreenClass = EscapeScreenRef.Class;
	}

	// HUD Class 대입.
	static ConstructorHelpers::FClassFinder<UACHUDWidget> ACHUDWidgetRef(TEXT("/Game/Project/UI/WBP_ACHUD.WBP_ACHUD_C"));
	if (ACHUDWidgetRef.Succeeded())
	{
		ACHUDWidgetClass = ACHUDWidgetRef.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACHUDWidgetRef is empty"));
	}

	//Role Screen 로드
	static ConstructorHelpers::FClassFinder<UACRoleScreen> RoleScreenRef(TEXT("/Game/Project/UI/GameStart/WBP_RoleScreen.WBP_RoleScreen_C"));
	if (RoleScreenRef.Succeeded())
	{
		RoleScreenClass = RoleScreenRef.Class;
	}

	// ===== 입력 관련 로드 =====
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContextRef(TEXT("/Game/Project/Input/IMC_Shoulder.IMC_Shoulder"));
	if (DefaultMappingContextRef.Succeeded())
	{
		DefaultMappingContext = DefaultMappingContextRef.Object;
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
	
	static ConstructorHelpers::FObjectFinder<UInputAction> DashActionRef(TEXT("/Game/Project/Input/Actions/IA_Dash.IA_Dash"));
	if (DashActionRef.Succeeded())
	{
		DashAction = DashActionRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> SprintActionRef(TEXT("/Game/Project/Input/Actions/IA_Sprint.IA_Sprint"));
	if (SprintActionRef.Succeeded())
	{
		SprintAction = SprintActionRef.Object;
	}

	// ===== 퀵슬롯 Input Action 로드 (하나만) =====
	static ConstructorHelpers::FObjectFinder<UInputAction> QuickSlotActionRef(TEXT("/Game/Project/Input/Actions/IA_QuickSlot.IA_QuickSlot"));
	if (QuickSlotActionRef.Succeeded())
	{
		QuickSlotAction = QuickSlotActionRef.Object;
	}

	// ===== 설정창 입력 로드 =====
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> SettingsMappingContextRef(TEXT("/Game/Project/Input/IMC_Settings.IMC_Settings"));
	if (SettingsMappingContextRef.Succeeded())
	{
		SettingsMappingContext = SettingsMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SettingsCloseActionRef(TEXT("/Game/Project/Input/Actions/IA_SettingsClose.IA_SettingsClose"));
	if (SettingsCloseActionRef.Succeeded())
	{
		SettingsCloseAction = SettingsCloseActionRef.Object;
	}

	// ===== 관전자 입력 로드 =====
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> SpectatorMappingContextRef(TEXT("/Game/Project/Input/IMC_Spectator.IMC_Spectator"));
	if (SpectatorMappingContextRef.Succeeded())
	{
		SpectatorMappingContext = SpectatorMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SpectatorChangeActionRef(TEXT("/Game/Project/Input/Actions/IA_SpectatorChange.IA_SpectatorChange"));
	if (SpectatorChangeActionRef.Succeeded())
	{
		SpectatorChangeAction = SpectatorChangeActionRef.Object;
	}

}

void AACMainPlayerController::PostInitializeComponents()
{
	AC_LOG(LogHY, Warning, TEXT("Begin"));
	Super::PostInitializeComponents();
	AC_LOG(LogHY, Warning, TEXT("End"));
}

void AACMainPlayerController::PostNetInit()
{
	AC_LOG(LogHY, Warning, TEXT("Begin"));
	Super::PostNetInit();
	AC_LOG(LogHY, Warning, TEXT("End"));
}

void AACMainPlayerController::BeginPlay()
{
	AC_LOG(LogHY, Warning, TEXT("Begin"));
	Super::BeginPlay();

	//ConsoleCommand(TEXT("show Collision"));
	ConsoleCommand(TEXT("Stat FPS"));
	UE_LOG(LogTemp, Warning, TEXT("AACMainPlayerController BeginPlay"));

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	if (IsLocalController() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("IsLocalController false"));
		return;
	}

	// 보이스 연결
	UACAdvancedFriendsGameInstance* GI = GetGameInstance<UACAdvancedFriendsGameInstance>();
	if (GI)
	{
		GI->TryStartVoice();
	}

	// 거리 기반 Voice 타이머 시작
	//StartProximityVoiceTimer();

	ACHUDWidget = CreateWidget<UACHUDWidget>(this, ACHUDWidgetClass);
	if (ACHUDWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ACHUDWidget is empty"));
		return;
	}

	// 뭐하는 함수인지 알아보기.
	ACHUDWidget->AddToViewport();

	ACHUDWidget->BindGameState();

	//if (HasAuthority())
	//{
		// 서버와 클라이언트 모두 바인딩 필요
	ACHUDWidget->BindPlayerState();
	//}

	RoleScreen = CreateWidget<UACRoleScreen>(this, RoleScreenClass);
	if (RoleScreen == nullptr)
	{
		return;
	}

	RoleScreen->AddToViewport();
	RoleScreen->OnFadeOutFinished.AddDynamic(this, &AACMainPlayerController::OnRoleFadeInFinished);
	if (HasAuthority())
	{
		ScreenSetRole();
	}
	AC_LOG(LogHY, Warning, TEXT("End"));

}

void AACMainPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 거리 기반 Voice 타이머 정지
	StopProximityVoiceTimer();

	Super::EndPlay(EndPlayReason);
}

void AACMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Enhanced Input Component 획득
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("EnhancedInputComponent is nullptr"));
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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AACMainPlayerController::HandleMove);
	}
	if (LookAction)
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AACMainPlayerController::HandleLook);
	}
	if (JumpAction)
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AACMainPlayerController::HandleJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AACMainPlayerController::HandleStopJumping);
	}
	if (InteractAction)
	{
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AACMainPlayerController::HandleInteractStart);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AACMainPlayerController::HandleInteractHold);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AACMainPlayerController::HandleInteractRelease);
	}
	if (ItemDropAction)
	{
		EnhancedInputComponent->BindAction(ItemDropAction, ETriggerEvent::Started, this, &AACMainPlayerController::HandleItemDrop);
	}
	if (MeleeAction)
	{
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Started, this, &AACMainPlayerController::HandleAttack);
	}
	
	// 캐릭터 스킬 - Dash
	if (DashAction)
	{
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AACMainPlayerController::HandleDash);
	}
	// 캐릭터 스킬 - Sprint
	if (SprintAction)
	{
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AACMainPlayerController::HandleSprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AACMainPlayerController::HandleSprintEnd);
		// EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &AACMainPlayerController::HandleSprintEnd);
	}
	
	if (SettingsCloseAction)
	{
		EnhancedInputComponent->BindAction(SettingsCloseAction, ETriggerEvent::Started, this, &AACMainPlayerController::HandleSettingsClose);
	}

	if (QuickSlotAction)
	{
		EnhancedInputComponent->BindAction(QuickSlotAction, ETriggerEvent::Started, this, &AACMainPlayerController::HandleQuickSlot);
	}

	if (SpectatorChangeAction)
	{
		EnhancedInputComponent->BindAction(SpectatorChangeAction, ETriggerEvent::Started, this, &AACMainPlayerController::HandleSpectatorChange);
	}
}

void AACMainPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ACHUDWidget->BindPlayerState();

	ScreenSetRole();
}

// ===== 입력 핸들러 구현 =====
void AACMainPlayerController::HandleMove(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter == nullptr)
	{
		return;
	}

	ControlledCharacter->Move(Value);
}

void AACMainPlayerController::HandleLook(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter == nullptr)
	{
		return;
	}

	ControlledCharacter->Look(Value);
}

void AACMainPlayerController::HandleJump(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter == nullptr)
	{
		return;
	}

	ControlledCharacter->Jump();
}

void AACMainPlayerController::HandleStopJumping(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter == nullptr)
	{
		return;
	}

	ControlledCharacter->StopJumping();
}

void AACMainPlayerController::HandleInteractStart(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter == nullptr)
	{
		return;
	}

	ControlledCharacter->InteractStarted();
}

void AACMainPlayerController::HandleInteractHold(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter == nullptr)
	{
		return;
	}

	ControlledCharacter->InteractHolding(GetWorld()->GetDeltaSeconds());
}

void AACMainPlayerController::HandleInteractRelease(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter == nullptr)
	{
		return;
	}

	ControlledCharacter->InteractReleased();
}

void AACMainPlayerController::HandleItemDrop(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter == nullptr)
	{
		return;
	}

	ControlledCharacter->ItemDrop();
}

void AACMainPlayerController::HandleAttack(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter == nullptr)
	{
		return;
	}

	ControlledCharacter->Attack();
}

void AACMainPlayerController::HandleSettingsClose(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter == nullptr)
	{
		return;
	}

	ControlledCharacter->SettingsClose();
}

void AACMainPlayerController::HandleSpectatorChange(const FInputActionValue& Value)
{
	AC_LOG(LogSY, Log, TEXT("Spectator Change!!"));

	if (IsLocalController() == false)
	{
		return;
	}

	//관전 대상 변경
	ServerSwitchToNextSpectateTarget();
}

void AACMainPlayerController::HandleDash(const struct FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter == nullptr)
	{
		AC_LOG(LogHY, Error, TEXT("ControlledCharacter is nullptr"));
		return;
	}

	ControlledCharacter->Dash(Value);
}

void AACMainPlayerController::HandleSprintStart(const struct FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter == nullptr)
	{
		AC_LOG(LogHY, Error, TEXT("ControlledCharacter is nullptr"));
		return;
	}
	AC_LOG(LogHY, Error, TEXT("Start Hi"));
	ControlledCharacter->Sprint(Value);
}

void AACMainPlayerController::HandleSprintEnd(const struct FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter == nullptr)
	{
		AC_LOG(LogHY, Error, TEXT("ControlledCharacter is nullptr"));
		return;
	}
	AC_LOG(LogHY, Error, TEXT("End Hi"));
	ControlledCharacter->Sprint(Value);
}

void AACMainPlayerController::HandleQuickSlot(const FInputActionValue& Value)
{
	if (ACHUDWidget == nullptr || ACHUDWidget->WBP_QuickSlot == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleQuickSlot: QuickSlot widget is null"));
		return;
	}

	// Value에서 슬롯 번호 가져오기 (1, 2, 3, ...)
	int32 SlotIndex = FMath::RoundToInt(Value.Get<float>());

	UE_LOG(LogTemp, Log, TEXT("QuickSlot pressed: %d"), SlotIndex);

	// 통합된 함수 호출
	ACHUDWidget->WBP_QuickSlot->ToggleSlotEquip(SlotIndex);
}

void AACMainPlayerController::ChangeInputMode(EInputMode NewMode)
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
		else if (NewMode == EInputMode::Spectator && SpectatorMappingContext)
		{
			Subsystem->AddMappingContext(SpectatorMappingContext, 0);
		}
		else
		{
			AC_LOG(LogSY, Log, TEXT("ChangeMode fail"));
		}
	}
}

void AACMainPlayerController::ClientOnEscapeSuccess_Implementation()
{
	//UI 변경
	EscapeScreen = CreateWidget<UUserWidget>(this, EscapeScreenClass);
	if (EscapeScreen == nullptr)
	{
		return;
	}
	EscapeScreen->AddToViewport();

	// IMC 변경
	ChangeInputMode(EInputMode::Spectator);
}

void AACMainPlayerController::ClientToggleShopWidget_Implementation(TSubclassOf<class UACShopWidget> WidgetClass)
{
	if (WidgetClass == nullptr)
	{
		UE_LOG(LogHG, Error, TEXT("ClientToggleShopWidget: WidgetClass is null"));
		return;
	}

	// 이미 위젯이 열려있으면 닫기
	if (CurrentShopWidget != nullptr && CurrentShopWidget->IsInViewport())
	{
		CloseShop();
	}
	else
	{
		// ===== 상점 열기 =====
		UACShopWidget* ShopWidget = CreateWidget<UACShopWidget>(GetWorld(), WidgetClass);

		if (ShopWidget != nullptr)
		{
			ShopWidget->AddToViewport();
			CurrentShopWidget = ShopWidget;

			// 델리게이트 바인딩
			ShopWidget->OnCloseRequested.AddDynamic(this, &AACMainPlayerController::CloseShop);

			// 상점용 카메라로 전환
			SetShopCamera();

			// 입력모드를 게임,UI로 변경
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(ShopWidget->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false); // 클릭해도 마우스 커서 계속 보이도록
			SetInputMode(InputMode);

			SetShowMouseCursor(true); // 마우스 커서 보이도록

			// 상점의 슬롯들 로드
			ShopWidget->LoadAndCreateSlots(TEXT("/Game/Project/Item/"));

			// 입력 모드 변경
			ChangeInputMode(EInputMode::Settings);
		}
	}
}

void AACMainPlayerController::CloseShop()
{
	if (CurrentShopWidget == nullptr || CurrentShopWidget->IsInViewport() == false)
	{
		// 이미 닫혀있음
		return;
	}

	// ===== 상점 닫기 =====
	CurrentShopWidget->RemoveFromParent();
	CurrentShopWidget = nullptr;

	// 카메라 복원
	RestoreOriginalCamera();

	// 마우스 숨기기
	SetShowMouseCursor(false);

	// ===== 입력 모드를 게임 모드로 전환 =====
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(false);
	SetInputMode(InputMode);

	// 입력 모드 변경
	ChangeInputMode(EInputMode::Sholder);
}

void AACMainPlayerController::CloseCCTV()
{
	if (CurrentCCTVWidget == nullptr || CurrentCCTVWidget->IsInViewport() == false)
	{
		return;
	}

	// ===== CCTV 닫기 =====
	CurrentCCTVWidget->RemoveFromParent();
	CurrentCCTVWidget = nullptr;

	SetShowMouseCursor(false);

	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(false);
	SetInputMode(InputMode);

	ChangeInputMode(EInputMode::Sholder);
}

void AACMainPlayerController::SetShopCamera()
{
	if (bShopCameraActive == true)
	{
		// 이미 상점 카메라 모드
		return;
	}

	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter == nullptr)
	{
		return;
	}

	// CameraBoom 가져오기
	USpringArmComponent* CameraBoom = ControlledCharacter->FindComponentByClass<USpringArmComponent>();
	if (CameraBoom == nullptr)
	{
		UE_LOG(LogHG, Error, TEXT("CameraBoom not found on Character"));
		return;
	}

	// ===== 현재 카메라 상태 저장 =====
	SavedControlRotation = GetControlRotation();
	SavedTargetArmLength = CameraBoom->TargetArmLength;
	SavedTargetOffset = CameraBoom->TargetOffset;
	SavedRelativeLocation = CameraBoom->GetRelativeLocation();
	SavedRelativeRotation = CameraBoom->GetRelativeRotation();
	bSavedUsePawnControlRotation = CameraBoom->bUsePawnControlRotation;

	// ===== 월드 기준 고정 각도로 카메라 설정 =====

	// CameraBoom이 컨트롤러 회전을 따라가지 않도록 설정
	CameraBoom->bUsePawnControlRotation = false;

	// 캐릭터 정면(월드 기준 북쪽)에서 약간 위에서 보도록 설정
	FRotator WorldFixedRotation = FRotator(-15.f, 0.f, 0.f);
	SetControlRotation(WorldFixedRotation);

	// CameraBoom의 로컬 회전을 설정 (캐릭터를 정면에서 봄)
	CameraBoom->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	CameraBoom->SetRelativeLocation(FVector(100.0f, 100.0f, 0.f));

	// 카메라 거리 및 오프셋 조정
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->TargetOffset = FVector(0.f, 0.f, 50.f);

	bShopCameraActive = true;
}

void AACMainPlayerController::RestoreOriginalCamera()
{
	if (bShopCameraActive == false)
	{
		return;  // 상점 카메라 모드가 아님
	}

	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter == nullptr)
	{
		return;
	}

	// CameraBoom 가져오기
	USpringArmComponent* CameraBoom = ControlledCharacter->FindComponentByClass<USpringArmComponent>();
	if (CameraBoom == nullptr)
	{
		return;
	}

	// ===== 원래 카메라 상태 복원 =====

	// 컨트롤러 회전 복원
	SetControlRotation(SavedControlRotation);

	// CameraBoom 설정 복원
	CameraBoom->bUsePawnControlRotation = bSavedUsePawnControlRotation;
	CameraBoom->SetRelativeRotation(SavedRelativeRotation);
	CameraBoom->SetRelativeLocation(SavedRelativeLocation);
	CameraBoom->TargetArmLength = SavedTargetArmLength;
	CameraBoom->TargetOffset = SavedTargetOffset;

	bShopCameraActive = false;
}

void AACMainPlayerController::OnRoleFadeInFinished()
{
	AC_LOG(LogSY, Log, TEXT("FadeOut"));
	RoleScreen->RemoveFromParent();
}

void AACMainPlayerController::ScreenSetRole()
{
	AACPlayerState* PS = GetPlayerState<AACPlayerState>();
	if (PS == nullptr)
	{
		return;
	}


	if (RoleScreen == nullptr)
	{
		return;
	}
	RoleScreen->SetRole(PS->PlayerRole);
	AC_LOG(LogSY, Log, TEXT("Set Role!"));
}

void AACMainPlayerController::ClientNotifySpectateTargetRemoved_Implementation(APawn* RemovedPawn)
{
	if (IsLocalController() == false)
	{
		return;
	}

	// 현재 관전 중인 대상 확인
	AActor* CurrentViewTarget = GetViewTarget();

	// 제거된 폰이 현재 관전 중인 대상인 경우
	if (CurrentViewTarget == RemovedPawn)
	{
		AC_LOG(LogSY, Log, TEXT("Current spectate target removed. Requesting auto-switch to next target."));

		// 서버에 다음 대상으로 전환 요청
		ServerSwitchToNextSpectateTarget();
	}
	else
	{
		AC_LOG(LogSY, Log, TEXT("Removed pawn is not current view target. No action needed."));
	}
}

void AACMainPlayerController::ServerSwitchToNextSpectateTarget_Implementation()
{
	AACMainGameState* GS = GetWorld()->GetGameState<AACMainGameState>();
	if (GS == nullptr)
	{
		AC_LOG(LogSY, Error, TEXT("GameState is nullptr"));
		return;
	}

	const TArray<TObjectPtr<APawn>>& Targets = GS->GetSpectatablePawns();

	// 관전 가능한 대상이 없는 경우
	if (Targets.Num() == 0)
	{
		AC_LOG(LogSY, Warning, TEXT("No spectatable targets available"));
		CurrentSpectateIndex = INDEX_NONE;
		return;
	}

	// 현재 인덱스 초기화
	if (CurrentSpectateIndex == INDEX_NONE || Targets.IsValidIndex(CurrentSpectateIndex) == false)
	{
		CurrentSpectateIndex = -1;
	}

	// 다음 유효한 대상 찾기
	int32 SearchCount = 0;

	do
	{
		// 다음 인덱스 이동
		CurrentSpectateIndex = (CurrentSpectateIndex + 1) % Targets.Num();
		SearchCount++;

		APawn* NextPawn = Targets[CurrentSpectateIndex];

		// 유효한 폰인지 확인
		if (NextPawn != nullptr && IsValid(NextPawn) == true)
		{
			// ViewTarget 설정
			SetViewTargetWithBlend(NextPawn, 0.0f);
			return;
		}

		// 모든 대상을 확인했는데 유효한 폰이 없는 경우
		if (SearchCount >= Targets.Num())
		{
			AC_LOG(LogSY, Warning, TEXT("No valid spectatable pawns found"));
			CurrentSpectateIndex = INDEX_NONE;
			return;
		}

	} while (SearchCount < Targets.Num());
}


void AACMainPlayerController::ServerStartSpectateOtherPlayer_Implementation()
{
	APawn* MyPawn = GetPawn();
	if (MyPawn == nullptr)
	{
		return;
	}

	MyPawn->SetActorHiddenInGame(true);
	MyPawn->SetActorEnableCollision(false);

	//관전 대상 선택
	ServerSwitchToNextSpectateTarget();

	//게임종료 조건 체크
	AACMainGameMode* GM = GetWorld()->GetAuthGameMode<AACMainGameMode>();
	if (GM == nullptr)
	{
		return;
	}

	GM->GetGameRuleManager()->CheckGameEndCondition();
}

void AACMainPlayerController::ClientToggleCCTVWidget_Implementation(TSubclassOf<class UACCCTVWidget> WidgetClass)
{
	if (WidgetClass == nullptr)
	{
		UE_LOG(LogHG, Error, TEXT("ClientToggleCCTVWidget: WidgetClass is null"));
		return;
	}

	// 이미 위젯이 열려있으면 닫기
	if (CurrentCCTVWidget != nullptr && CurrentCCTVWidget->IsInViewport())
	{
		CloseCCTV();
	}
	else
	{
		// ===== CCTV 열기 =====
		UACCCTVWidget* CCTVWidget = CreateWidget<UACCCTVWidget>(GetWorld(), WidgetClass);

		if (CCTVWidget != nullptr)
		{
			CCTVWidget->AddToViewport();
			CurrentCCTVWidget = CCTVWidget;

			// 입력모드를 게임,UI로 변경
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(CCTVWidget->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);
			SetInputMode(InputMode);

			SetShowMouseCursor(true);

			ChangeInputMode(EInputMode::Settings);
		}
	}
}

void AACMainPlayerController::ShowInteractProgress(const FString& TargetName)
{
	//UE_LOG(LogSW, Log, TEXT("Showing"));
	if (ACHUDWidget == nullptr)
	{
		return;
	}
	if (ACHUDWidget->WBP_InteractProgress == nullptr)
	{
		return;
	}

	ACHUDWidget->WBP_InteractProgress->SetTargetName(TargetName);
	ACHUDWidget->WBP_InteractProgress->SetProgress(0.f);
	ACHUDWidget->WBP_InteractProgress->ShowWidget();
}

void AACMainPlayerController::UpdateInteractProgress(float Progress)
{
	if (ACHUDWidget == nullptr)
	{
		return;
	}
	if (ACHUDWidget->WBP_InteractProgress == nullptr)
	{
		return;
	}

	ACHUDWidget->WBP_InteractProgress->SetProgress(Progress);
}

void AACMainPlayerController::HideInteractProgress()
{
	//UE_LOG(LogSW, Log, TEXT("Hiding"));
	if (ACHUDWidget == nullptr)
	{
		return;
	}
	if (ACHUDWidget->WBP_InteractProgress == nullptr)
	{
		return;
	}

	ACHUDWidget->WBP_InteractProgress->HideWidget();
}

#pragma region Proximity Voice (거리 기반 보이스)
void AACMainPlayerController::StartProximityVoiceTimer()
{
	if (IsLocalController() == false)
	{
		return;
	}

	// 기존 타이머가 있으면 정지
	StopProximityVoiceTimer();

	// 타이머 시작
	GetWorldTimerManager().SetTimer(
		VoiceUpdateTimerHandle,
		this,
		&AACMainPlayerController::UpdateProximityVoice,
		VoiceUpdateInterval,
		true  // 반복 실행
	);

	AC_LOG(LogSY, Log, TEXT("Proximity Voice Timer Started (Interval: %.2f sec, MaxDistance: %.0f cm)"),
		VoiceUpdateInterval, VoiceMaxDistance);
}

void AACMainPlayerController::StopProximityVoiceTimer()
{
	if (VoiceUpdateTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(VoiceUpdateTimerHandle);
		VoiceUpdateTimerHandle.Invalidate();
	}

	// 음소거 목록 초기화
	MutedPlayers.Empty();
}

void AACMainPlayerController::UpdateProximityVoice()
{
	// 내 폰 가져오기
	APawn* MyPawn = GetPawn();
	if (MyPawn == nullptr)
	{
		return;
	}

	// Voice 인터페이스 가져오기
	IOnlineVoicePtr Voice = Online::GetVoiceInterface();
	if (Voice.IsValid() == false)
	{
		return;
	}

	// GameState 가져오기
	AGameStateBase* GameState = GetWorld()->GetGameState();
	if (GameState == nullptr)
	{
		return;
	}

	FVector MyLocation = MyPawn->GetActorLocation();

	// 모든 PlayerState 순회
	for (APlayerState* PS : GameState->PlayerArray)
	{
		// 자기 자신 스킵
		if (PS == nullptr || PS == PlayerState)
		{
			continue;
		}

		// 상대방 폰 가져오기
		APawn* OtherPawn = PS->GetPawn();
		if (OtherPawn == nullptr)
		{
			continue;
		}

		// 상대방 NetId 가져오기
		FUniqueNetIdRepl OtherNetId = PS->GetUniqueId();
		if (OtherNetId.IsValid() == false)
		{
			continue;
		}

		// 거리 계산
		float Distance = FVector::Dist(MyLocation, OtherPawn->GetActorLocation());

		// TODO: 무전기 로직 추가 예정
		// bool bBothHaveWalkieTalkie = CheckBothHaveWalkieTalkie(PS);
		bool bBothHaveWalkieTalkie = false;

		if (bBothHaveWalkieTalkie)
		{
			// 무전기 보유 시: 거리 상관없이 음소거 해제
			if (MutedPlayers.Contains(OtherNetId))
			{
				Voice->UnmuteRemoteTalker(0, *OtherNetId, false);
				MutedPlayers.Remove(OtherNetId);
				AC_LOG(LogSY, Verbose, TEXT("Voice Unmuted (WalkieTalkie): %s"), *PS->GetPlayerName());
			}
		}
		else if (Distance > VoiceMaxDistance)
		{
			// 거리가 멀면: 음소거
			if (MutedPlayers.Contains(OtherNetId) == false)
			{
				Voice->MuteRemoteTalker(0, *OtherNetId, false);
				MutedPlayers.Add(OtherNetId);
				AC_LOG(LogSY, Verbose, TEXT("Voice Muted: %s (Distance: %.0f cm)"), *PS->GetPlayerName(), Distance);
			}
		}
		else
		{
			// 거리가 가까우면: 음소거 해제
			if (MutedPlayers.Contains(OtherNetId))
			{
				Voice->UnmuteRemoteTalker(0, *OtherNetId, false);
				MutedPlayers.Remove(OtherNetId);
				AC_LOG(LogSY, Verbose, TEXT("Voice Unmuted: %s (Distance: %.0f cm)"), *PS->GetPlayerName(), Distance);
			}
		}
	}
}
#pragma endregion
