
#include "ACCharacter.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "NavigationSystem.h"
#include "AI/ACCitizenAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Item/ACEscapeMissionBomb.h"
#include "Kismet/GameplayStatics.h"

#include "Component/ACInteractableComponent.h"

#include "AnimalCrime.h"

#include "Game/ACLobbyPlayerController.h"

#include "Component/ACShopComponent.h"
#include "UI/ACShopWidget.h"


AACCharacter::AACCharacter()
{
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(35.f, 90.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	//캐릭터 무브먼트
	auto Move = GetCharacterMovement();
	Move->MaxWalkSpeed = 600.f;
	Move->JumpZVelocity = 600.f;
	Move->AirControl = 0.3f;
	Move->bOrientRotationToMovement = true;
	Move->RotationRate = FRotator(0.f, 500.f, 0.f);

	//스켈레탈 메시
	USkeletalMeshComponent* MeshComp = GetMesh();
	MeshComp->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Body_010.SK_Body_010")));
	MeshComp->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	MeshComp->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	MeshComp->SetReceivesDecals(false);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimRef(TEXT("/Script/Engine.AnimBlueprint'/Game/Project/Character/ABP_ACPlayerHena.ABP_ACPlayerHena_C'"));
	if (AnimRef.Succeeded())
	{
		MeshComp->SetAnimInstanceClass(AnimRef.Class);
	}

	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HeadMeshRef(TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Hat_049.SK_Hat_049"));
	if (HeadMeshRef.Succeeded() == true)
	{
		HeadMesh->SetSkeletalMesh(HeadMeshRef.Object);
	}
	HeadMesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	HeadMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	HeadMesh->SetupAttachment(RootComponent);
	HeadMesh->SetLeaderPoseComponent(MeshComp);
	HeadMesh->SetReceivesDecals(false);
	
	FaceMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FaceMeshRef(TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Male_emotion_usual_001.SK_Male_emotion_usual_001"));
	if (FaceMeshRef.Succeeded() == true)
	{
		FaceMesh->SetSkeletalMesh(FaceMeshRef.Object);
	}
	FaceMesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	FaceMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	FaceMesh->SetupAttachment(RootComponent);
	FaceMesh->SetLeaderPoseComponent(MeshComp);
	FaceMesh->SetReceivesDecals(false);

	TopMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Top"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TopMeshRef(TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Outerwear_036.SK_Outerwear_036"));
	if (TopMeshRef.Succeeded() == true)
	{
		TopMesh->SetSkeletalMesh(TopMeshRef.Object);
	}
	TopMesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	TopMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	TopMesh->SetupAttachment(RootComponent);
	TopMesh->SetLeaderPoseComponent(MeshComp);
	TopMesh->SetReceivesDecals(false);

	BottomMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bottom"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BottomMeshRef(TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Pants_014.SK_Pants_014"));
	if (BottomMeshRef.Succeeded() == true)
	{
		BottomMesh->SetSkeletalMesh(BottomMeshRef.Object);
	}
	BottomMesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	BottomMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	BottomMesh->SetupAttachment(RootComponent);
	BottomMesh->SetLeaderPoseComponent(MeshComp);
	BottomMesh->SetReceivesDecals(false);

	ShoesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoes"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ShoesMeshRef(TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Shoe_Slippers_005.SK_Shoe_Slippers_005"));
	if (ShoesMeshRef.Succeeded() == true)
	{
		ShoesMesh->SetSkeletalMesh(ShoesMeshRef.Object);
	}
	ShoesMesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	ShoesMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	ShoesMesh->SetupAttachment(RootComponent);
	ShoesMesh->SetLeaderPoseComponent(MeshComp);
	ShoesMesh->SetReceivesDecals(false);

	FaceAccMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FaceAcc"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FaceAccMeshRef(TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Moustache_002.SK_Moustache_002"));
	if (FaceAccMeshRef.Succeeded() == true)
	{
		FaceAccMesh->SetSkeletalMesh(FaceAccMeshRef.Object);
	}
	FaceAccMesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	FaceAccMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	FaceAccMesh->SetupAttachment(RootComponent);
	FaceAccMesh->SetLeaderPoseComponent(MeshComp);
	FaceAccMesh->SetReceivesDecals(false);

	//카메라
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetOffset = FVector(0.f, 60.f, 20.f); //솔더뷰, Y축으로 오른쪽으로 50

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	InteractBoxComponent = CreateDefaultSubobject<UACInteractableComponent>(TEXT("InteractBoxComponent"));
	InteractBoxComponent->SetupAttachment(RootComponent);

	//기본 키 입력
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Game/Project/Input/IMC_Shoulder.IMC_Shoulder"));
	if (InputMappingContextRef.Succeeded())
	{
		DefaultMappingContext = InputMappingContextRef.Object;
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
	
	// 
	static ConstructorHelpers::FObjectFinder<UInputAction> MeleeActionRef(TEXT("/Game/Project/Input/Actions/IA_Attack.IA_Attack"));
	if (MeleeActionRef.Succeeded())
	{
		MeleeAction = MeleeActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SteamFriendListActionRef(TEXT("/Game/Project/Input/Actions/IA_SteamFriendList.IA_SteamFriendList"));
	if (SteamFriendListActionRef.Succeeded())
	{
		SteamFriendListAction = SteamFriendListActionRef.Object;
	}

	//설정창 키 입력
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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeleeMontageRef(TEXT("/Game/Project/Character/AM_Melee.AM_Melee"));
	if (MeleeMontageRef.Succeeded())
	{
		MeleeMontage = MeleeMontageRef.Object;
	}
	
	// ShopComponent 생성
	ShopComponent = CreateDefaultSubobject<UACShopComponent>(TEXT("ShopComponent"));
	
	GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
}


void AACCharacter::BeginPlay()
{

	Super::BeginPlay();
}

void AACCharacter::ChangeInputMode(EInputMode NewMode)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC == nullptr)
	{
		return;
	}
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (Subsystem == nullptr)
	{
		return;
	}

	// 현재 등록된 모든 매핑 제거
	Subsystem->ClearAllMappings();

	// 새로운 Context 추가
	switch (NewMode)
	{
	case EInputMode::Sholder:
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		break;
	case EInputMode::Settings:
		Subsystem->AddMappingContext(SettingsMappingContext, 0);
		break;
	default:
		break;
	}
}

void AACCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AACCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

/**
    @brief 상호작용 키(E) 키 입력시, 구현 코드
    @param Value - 
**/
void AACCharacter::Interact(const FInputActionValue& Value)
{
	//AC_LOG(LogSW, Log, TEXT("Interact Pressed"));

	if (SortNearInteractables() == true)
	{
		ServerInteract();
	}	
}

void AACCharacter::ItemDrop(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("ItemDrop!!"));
}

void AACCharacter::Attack()
{
	// 현재 공격 중인지 확인. 
	if (CheckProcessAttack() == true)
	{
		return;
	}
	
	// Listen Server에 존재하는 클라이언트 경우
	if (HasAuthority())
	{
		PerformAttackTrace();
	}
	// Listen Server에 접속한 클라이언트 경우
	else
	{
		ServerAttack();
	}
}

void AACCharacter::SetSteamFriendsList(const FInputActionValue& Value)
{
	AACLobbyPlayerController* PC = Cast<AACLobbyPlayerController>(GetController());
	if (PC == nullptr)
	{
		return;
	}

	//설정창이 꺼져있으면 스팀친구창 오픈, 스팀 친구창이 켜져있으면 끄기, 다른 설정창이면 아무것도 안함.
	if (SettingMode == ESettingMode::None)
	{
		PC->SteamFriendListToggle(true);
		ChangeInputMode(EInputMode::Settings);
		SettingMode = ESettingMode::SteamFriendList;
	}
	else if (SettingMode == ESettingMode::SteamFriendList)
	{
		PC->SteamFriendListToggle(false);
		ChangeInputMode(EInputMode::Sholder);
		SettingMode = ESettingMode::None;
	}

}

void AACCharacter::SettingsClose(const FInputActionValue& Value)
{
	switch (SettingMode)
	{
	case ESettingMode::None:
		break;
	case ESettingMode::Default:
		break;
	case ESettingMode::SteamFriendList:
		SetSteamFriendsList(Value);
		break;

	default:
		break;
	}
	
	//ChangeInputMode(EInputMode::Sholder);
	//SettingMode = ESettingMode::None;
}

void AACCharacter::ServerInteract_Implementation()
{
	if (NearInteractables.Num() == 0)
	{
		AC_LOG(LogSW, Log, TEXT("No Near Interactables!!"));
		return;
	}

	for (AActor* Target : NearInteractables)
	{
		AC_LOG(LogSW, Log, TEXT("%s Selected!"), *Target->GetName());

		IACInteractInterface* Interactable = Cast<IACInteractInterface>(Target);
		if (Interactable == nullptr)
		{
			continue;
		}

		// 1. 상호작용 가능한지 체크( 현재 캐릭터 roll과 상호작용 가능한 물체인가?)
		if (Interactable->CanInteract(this) == false)
		{
			continue;
		}

		// todo: 2. 거리 체크 (치트 방지)	???
		//float Dist = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
		//if (Dist > 300.f) return;

		// 3. 상호작용 실행
		Interactable->OnInteract(this);
		return;
	}

	// 여기까지 오면 배열에 상호작용 인터페이스 구현한 액터가 없음.
	AC_LOG(LogSW, Log, TEXT("No Actor has Interface!!"));
}

bool AACCharacter::CheckProcessAttack() const
{
	return bAttackFlag;
}

void AACCharacter::ChangeAttackTrue()
{
	bAttackFlag = true;
}

void AACCharacter::ChangeAttackFalse()
{
	bAttackFlag = false;
}

void AACCharacter::PerformAttackTrace()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}

	MulticastPlayAttackMontage();
}

void AACCharacter::AttackHitCheck()
{
	// 캡슐 크기
	float CapsuleRadius = 30.0f;
	float CapsuleHalfHeight = 60.0f;

	// 트레이스 길이
	float TraceDistance = 200.0f;

	// 시작 위치 = 캐릭터 위치
	FVector Start = GetActorLocation();
                
	// 끝 위치 = 캐릭터 앞 방향 * 거리
	FVector Forward = GetActorForwardVector();
	FVector End = Start + Forward * TraceDistance;

	// 충돌 파라미터 설정
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);   // 자기 자신 무시
	Params.bTraceComplex = false;
	Params.bReturnPhysicalMaterial = false;

	FHitResult Hit;

	bool bHit = GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight), Params);

	// 디버그: 캡슐 그리기
	DrawDebugCapsule(GetWorld(), (Start + End) * 0.5f, CapsuleHalfHeight, CapsuleRadius, FRotationMatrix::MakeFromZ(End - Start).ToQuat(), bHit ? FColor::Red : FColor::Green, false, 1.0f);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName());
		UGameplayStatics::ApplyDamage(Hit.GetActor(),30.0f, GetController(),this, nullptr);
	}
}

bool AACCharacter::CanInteract(AACCharacter* ACPlayer)
{
	// 시민과 시민은 상호작용 가능하다?
	return true;
}

void AACCharacter::OnInteract(AACCharacter* ACPlayer)
{
	ShowInteractDebug(ACPlayer);
}

FString AACCharacter::GetInteractableName() const
{
	return TEXT("ACCharacter");
}


void AACCharacter::AddInteractable(AActor* Interactor)
{
	//ensureAlways(Interactor);
	NearInteractables.AddUnique(Interactor);
}

void AACCharacter::RemoveInteractable(AActor* Interactor)
{
	//ensureAlways(Interactor);
	NearInteractables.Remove(Interactor);
}

/**
    @brief  NearInteractables Array의 Actor들을 플레이어와 거리가 가까운 순서로 Sort. Sort 여부를 반환.
    @retval  - NearInteractables가 Sort되었으면 true, 아니면 false 반환
**/
bool AACCharacter::SortNearInteractables()
{
	if (NearInteractables.Num() == 0)
	{
		AC_LOG(LogSW, Log, TEXT("No Close Interactables!!"));
		return false;
	}

	if (NearInteractables.Num() == 1)
	{
		AC_LOG(LogSW, Log, TEXT("NO NEED TO SORT!!"));
		return true;
	}

	const FVector PlayerLocation  = GetActorLocation();
	NearInteractables.Sort([PlayerLocation](const AActor& A, const AActor& B)
		{
			float DistA = FVector::DistSquared(PlayerLocation, A.GetActorLocation());
			float DistB = FVector::DistSquared(PlayerLocation, B.GetActorLocation());
			return DistA < DistB;
		});

	return true;
}

void AACCharacter::MulticastPlayAttackMontage_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Multicast"));
	if (MeleeMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MeleeMontage, 2.0f);
	}
}

void AACCharacter::ServerAttack_Implementation()
{
	PerformAttackTrace(); // 서버에서만 실행됨
}

void AACCharacter::ServerItemDrop_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Server ItemDrop!!"));
}


void AACCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC == nullptr)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (InputSystem == nullptr)
	{
		return;
	}

	InputSystem->AddMappingContext(DefaultMappingContext, 0);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AACCharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AACCharacter::Look);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AACCharacter::Interact);
	EnhancedInputComponent->BindAction(ItemDropAction, ETriggerEvent::Triggered, this, &AACCharacter::ItemDrop);
	EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Triggered, this, &AACCharacter::Attack);
	EnhancedInputComponent->BindAction(SteamFriendListAction, ETriggerEvent::Triggered, this, &AACCharacter::SetSteamFriendsList);
	EnhancedInputComponent->BindAction(SettingsCloseAction, ETriggerEvent::Triggered, this, &AACCharacter::SettingsClose);
}


EACCharacterType AACCharacter::GetCharacterType()
{
	return EACCharacterType::Citizen;
}

void AACCharacter::ClientToggleShopWidget_Implementation(TSubclassOf<class UACShopWidget> WidgetClass)
{
	if (WidgetClass == nullptr)
	{
		UE_LOG(LogHG, Error, TEXT("ClientToggleShopWidget: WidgetClass is null"));
		return;
	}

	APlayerController* PC = GetController<APlayerController>();
	if (PC == nullptr)
	{
		return;
	}

	// 이미 위젯이 열려있으면 닫기
	if (CurrentShopWidget != nullptr && CurrentShopWidget->IsInViewport())
	{
		// ===== 상점 닫기 =====
		UE_LOG(LogHG, Log, TEXT("Client: Closing Shop UI for %s"), *GetName());

		CurrentShopWidget->RemoveFromParent();
		CurrentShopWidget = nullptr;

		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetIgnoreMoveInput(false);
		PC->SetIgnoreLookInput(false);
	}
	else
	{
		// ===== 상점 열기 =====
		UE_LOG(LogHG, Log, TEXT("Client: Opening Shop UI for %s"), *GetName());

		UACShopWidget* ShopWidget = CreateWidget<UACShopWidget>(GetWorld(), WidgetClass);

		if (ShopWidget != nullptr)
		{
			ShopWidget->AddToViewport();
			CurrentShopWidget = ShopWidget;

			PC->SetShowMouseCursor(true);

			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(ShopWidget->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputMode);

			PC->SetIgnoreMoveInput(true);
			PC->SetIgnoreLookInput(true);

			ShopWidget->LoadAndCreateSlots(TEXT("/Game/Project/Item/"));
		}
	}
}