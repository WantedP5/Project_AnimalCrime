
#include "ACCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Item/ACEscapeMissionBomb.h"
#include "Kismet/GameplayStatics.h"

#include "Component/ACShopComponent.h"
#include "AnimalCrime.h"

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

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimRef(TEXT("/Script/Engine.AnimBlueprint'/Game/Project/Character/ABP_ACPlayer.ABP_ACPlayer_C'"));
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

	//입력
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

void AACCharacter::Interact(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Interact!!"));
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

		//Hit.GetActor()->Destroy();
		UGameplayStatics::ApplyDamage(Hit.GetActor(),30.0f, GetController(),this, nullptr);
	}
	
	MulticastPlayAttackMontage();
	//AnimInstance->Montage_Play(MeleeMontage);
}

void AACCharacter::MulticastPlayAttackMontage_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Multicast"));
	if (MeleeMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MeleeMontage);
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
}
