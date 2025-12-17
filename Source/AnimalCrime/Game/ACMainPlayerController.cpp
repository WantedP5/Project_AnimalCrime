
#include "ACMainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "ACTitlePlayerController.h"
#include "UI/Shop/ACShopWidget.h"
#include "AnimalCrime.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/ACCharacter.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "UI/Score/ACHUDWidget.h"

AACMainPlayerController::AACMainPlayerController()
{
	//탈출 스크린 로드
	static ConstructorHelpers::FClassFinder<UUserWidget> EscapeScreenRef(TEXT("/Game/Project/UI/WBP_EscapeText.WBP_EscapeText_C"));
	if (EscapeScreenRef.Succeeded())
	{
		EscapeScreenClass = EscapeScreenRef.Class;
	}
	
	// HUD Class 대입.
	static ConstructorHelpers::FClassFinder<UACHUDWidget> ACHUDWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Project/UI/GameScore/WBP_ACHUD.WBP_ACHUD_C'"));
	if (ACHUDWidgetRef.Succeeded())
	{
		ACHUDWidgetClass = ACHUDWidgetRef.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACHUDWidgetRef is empty"));
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
}

void AACMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//ConsoleCommand(TEXT("show Collision"));
	ConsoleCommand(TEXT("Stat FPS"));
	UE_LOG(LogTemp, Warning, TEXT("AACMainPlayerController BeginPlay"));
	
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	if (!IsLocalController())
	{
		return;   // 🔥 이 줄이 핵심
	}
	ACHUDWidget = CreateWidget<UACHUDWidget>(this, ACHUDWidgetClass);
	if (ACHUDWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ACHUDWidget is empty"));
		return ;
	}
	
	// 뭐하는 함수인지 알아보기.
	ACHUDWidget->AddToViewport();
	
	ACHUDWidget->BindGameState();
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
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AACMainPlayerController::HandleInteract);
	}
	if (ItemDropAction)
	{
		EnhancedInputComponent->BindAction(ItemDropAction, ETriggerEvent::Started, this, &AACMainPlayerController::HandleItemDrop);
	}
	if (MeleeAction)
	{
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Started, this, &AACMainPlayerController::HandleAttack);
	}
	if (SettingsCloseAction)
	{
		EnhancedInputComponent->BindAction(SettingsCloseAction, ETriggerEvent::Started, this, &AACMainPlayerController::HandleSettingsClose);
	}
}

// ===== 입력 핸들러 구현 =====
void AACMainPlayerController::HandleMove(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter)
	{
		ControlledCharacter->Move(Value);
	}
}

void AACMainPlayerController::HandleLook(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter)
	{
		ControlledCharacter->Look(Value);
	}
}

void AACMainPlayerController::HandleJump(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter)
	{
		ControlledCharacter->Jump();
	}
}

void AACMainPlayerController::HandleStopJumping(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter)
	{
		ControlledCharacter->StopJumping();
	}
}

void AACMainPlayerController::HandleInteract(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter)
	{
		ControlledCharacter->Interact(Value);
	}
}

void AACMainPlayerController::HandleItemDrop(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter)
	{
		ControlledCharacter->ItemDrop(Value);
	}
}

void AACMainPlayerController::HandleAttack(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter)
	{
		ControlledCharacter->Attack();
	}
}

void AACMainPlayerController::HandleSettingsClose(const FInputActionValue& Value)
{
	AACCharacter* ControlledCharacter = GetPawn<AACCharacter>();
	if (ControlledCharacter)
	{
		ControlledCharacter->SettingsClose(Value);
	}
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
	}
}

void AACMainPlayerController::ShowEscapeUI_Implementation()
{
    EscapeScreen = CreateWidget<UUserWidget>(this, EscapeScreenClass);
    if (EscapeScreen == nullptr)
    {
        return;
    }
    EscapeScreen->AddToViewport();
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
        UE_LOG(LogHG, Log, TEXT("Client: Opening Shop UI"));

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
    UE_LOG(LogHG, Log, TEXT("Client: Closing Shop UI"));

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