
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

#include "Component/ACShopComponent.h"
#include "UI/Shop/ACShopWidget.h"
#include "Game/ACMainPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/ACMoneyComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Net/UnrealNetwork.h"
#include "Objects/MoneyData.h"

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

	InteractBoxComponent = CreateDefaultSubobject<UACInteractableComponent>(TEXT("InteractBoxComponent"));
	InteractBoxComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeleeMontageRef(TEXT("/Game/Project/Character/AM_Melee.AM_Melee"));
	if (MeleeMontageRef.Succeeded())
	{
		MeleeMontage = MeleeMontageRef.Object;
	}
	
	// ShopComponent 생성
	ShopComponent = CreateDefaultSubobject<UACShopComponent>(TEXT("ShopComponent"));
	
	GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
	MoneyComp = CreateDefaultSubobject<UACMoneyComponent>(TEXT("MoneyComponent"));
	
	
	bReplicates = true;
}


void AACCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
	CharacterState = ECharacterState::Free;
	
	// @Todo 변경 필요. Mafia와 Police 구분이 안감.
    // Police와 Mafia는 각자의 BeginPlay에서 초기화
	// ※ 얘도 확인 했으면 지워주세요
	//MoneyComp->InitMoneyComponent(EMoneyType::MoneyMafiaType);
}

void AACCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AACCharacter, CharacterState);
}

void AACCharacter::ChangeInputMode(EInputMode NewMode)
{
	AACMainPlayerController* PC = Cast<AACMainPlayerController>(GetController());
	if (PC)
	{
		PC->ChangeInputMode(NewMode);
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

void AACCharacter::InteractStarted()
{
	//AC_LOG(LogSW, Log, TEXT("Interact Pressed"));

	if (SortNearInteractables() == true)
	{
		// 정렬된 NearInteractables에서 서버로 보내줄 하나의 액터를 찾는다.
		for (AActor* Target : NearInteractables)
		{
			IACInteractInterface* Interactable = Cast<IACInteractInterface>(Target);
			if (Interactable == nullptr)
			{
				AC_LOG(LogSW, Log, TEXT("%s has No Interface!!"), *Target->GetName());
				continue;
			}

			// 클라에서 상호작용 가능한지 체크
			if (Interactable->CanInteract(this) == false)
			{
				AC_LOG(LogSW, Log, TEXT("%s Cant Interact with %s!!"), *Target->GetName(), *GetName());
				continue;
			}

			// 클라이언트에서 상호작용이 가능한지 판단이 완료되면 서버를 호출한다.
			RequiredHoldTime = Interactable->GetRequiredHoldTime();

			// 0초면 즉시 상호작용
			if (RequiredHoldTime <= KINDA_SMALL_NUMBER)
			{
				ServerInteract(Target);
			}
			// 0초보다 크면 홀드 상호작용
			else
			{
				// 홀드 시작
				bIsHoldingInteract = true;
				CurrentHoldTarget = Target;
				CurrentHoldTime = 0.f;

				// UI 표시
				AACMainPlayerController* PC = Cast<AACMainPlayerController>(GetController());
				if (PC)
				{
					UE_LOG(LogSW, Log, TEXT("Show start"));
					PC->ShowInteractProgress(CurrentHoldTarget->GetName());
				}
			}
			
			return;
		}
	}
}

void AACCharacter::InteractHolding(const float DeltaTime)
{
	if (bIsHoldingInteract == false)
	{
		ResetHoldInteract();
		return;
	}
	//todo: isvalid?
	if (CurrentHoldTarget == nullptr)
	{
		InteractReleased();
		return;
	}
	if (!NearInteractables.Contains(CurrentHoldTarget))
	{
		InteractReleased();
		return;
	}


	// 시간 누적
	CurrentHoldTime += DeltaTime;
	//AC_LOG(LogSW, Log, TEXT("%s Holding at %f"), *CurrentHoldTarget->GetName(), CurrentHoldTime);

	// UI 업데이트
	AACMainPlayerController* PC = Cast<AACMainPlayerController>(GetController());
	if (PC)
	{
		PC->UpdateInteractProgress(GetHoldProgress());
	}

	// 완료 체크
	if (CurrentHoldTime >= RequiredHoldTime)
	{
		ServerInteract(CurrentHoldTarget);
		InteractReleased();
	}

}

void AACCharacter::InteractReleased()
{
	if (bIsHoldingInteract == false)
	{
		return;
	}

	// UI 표시
	AACMainPlayerController* PC = Cast<AACMainPlayerController>(GetController());
	if (PC)
	{
		UE_LOG(LogSW, Log, TEXT("Hide start"));
		PC->HideInteractProgress();
	}

	ResetHoldInteract();
}

void AACCharacter::ItemDrop()
{
	UE_LOG(LogTemp, Log, TEXT("ItemDrop!!"));
}

void AACCharacter::Attack()
{
	if (CharacterState == ECharacterState::Stun || CharacterState == ECharacterState::Prison)
	{
		return ;
	}
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

void AACCharacter::SettingsClose()
{
	//자식에서 추가로 구현
	switch (SettingMode)
	{
	case ESettingMode::None:
		break;
	case ESettingMode::Default:
		break;
	default:
		break;
	}
}

void AACCharacter::Jump()
{
	// Case 스턴 상태일 경우 
	if (CharacterState == ECharacterState::Stun)
	{
		return;
	}
	
	// Case 감옥 상태일 경우 
	if (CharacterState == ECharacterState::Prison)
	{
		return;
	}
	
	Super::Jump();
}

void AACCharacter::ServerInteract_Implementation(AActor* Target)
{
	if (Target == nullptr)
	{
		AC_LOG(LogSW, Log, TEXT("NULL!!"));
		return;
	}

	if (IsValid(Target) == false)
	{
		AC_LOG(LogSW, Log, TEXT("Interactable Deleted!!"));
		return;
	}

	IACInteractInterface* Interactable = Cast<IACInteractInterface>(Target);
	if (Interactable == nullptr)
	{
		AC_LOG(LogSW, Log, TEXT("%s has No Interface!!"), *Target->GetName());
		return;
	}

	// 상호작용 가능한지 서버에서 확실하게 체크
	if (Interactable->CanInteract(this) == false)
	{
		AC_LOG(LogSW, Log, TEXT("%s Cant Interact with %s!!"), *Target->GetName(), *GetName());
		return;
	}

	Interactable->OnInteract(this);
	AC_LOG(LogSW, Log, TEXT("%s Interacted with %s!!"), *Target->GetName(), *GetName());
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
	// // 캡슐 크기
	// float CapsuleRadius = 30.0f;
	// float CapsuleHalfHeight = 60.0f;
	//
	// // 트레이스 길이
	// float TraceDistance = 200.0f;
	//
	// // 시작 위치 = 캐릭터 위치
	// FVector Start = GetActorLocation();
 //                
	// // 끝 위치 = 캐릭터 앞 방향 * 거리
	// FVector Forward = GetActorForwardVector();
	// FVector End = Start + Forward * TraceDistance;
	//
	// // 충돌 파라미터 설정
	// FCollisionQueryParams Params;
	// Params.AddIgnoredActor(this);   // 자기 자신 무시
	// Params.bTraceComplex = false;
	// Params.bReturnPhysicalMaterial = false;
	//
	// FHitResult Hit;
	//
	// // bool bHit = GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_GameTraceChannel2 | ECC_GameTraceChannel4, FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight), Params);
	//
	// FCollisionObjectQueryParams ObjectParams;
	// ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
	// ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel5);
	//
	// bool bHit = GetWorld()->SweepSingleByObjectType(
	// 	Hit,
	// 	Start,
	// 	End,
	// 	FQuat::Identity,
	// 	ObjectParams,
	// 	FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),
	// 	Params
	// );
	//
	// // 디버그: 캡슐 그리기
	// DrawDebugCapsule(GetWorld(), (Start + End) * 0.5f, CapsuleHalfHeight, CapsuleRadius, FRotationMatrix::MakeFromZ(End - Start).ToQuat(), bHit ? FColor::Red : FColor::Green, false, 1.0f);
	//
	// if (bHit)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName());
	// 	UGameplayStatics::ApplyDamage(Hit.GetActor(),30.0f, GetController(),this, nullptr);
	// }
}

bool AACCharacter::CanInteract(AACCharacter* ACPlayer)
{
	// 시민과 시민은 상호작용 가능하다?
	return true;
}

void AACCharacter::OnInteract(AACCharacter* ACPlayer)
{
	ShowInteractDebug(ACPlayer, GetName());
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

bool AACCharacter::SortNearInteractables()
{
	for (int32 i = NearInteractables.Num() - 1; i >= 0; --i)
	{
		if (IsValid(NearInteractables[i]) == true)
		{
			continue;
		}

		NearInteractables.RemoveAt(i);
	}

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

void AACCharacter::OnRep_CharacterState()
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp == nullptr)
	{
		return;
	}
	
	switch (CharacterState)
	{
	case ECharacterState::Stun:
		{
			MoveComp->MaxWalkSpeed = 10.f;
			MoveComp->JumpZVelocity = 0.f;
			break;
		}
	case ECharacterState::Free:
		{
			MoveComp->MaxWalkSpeed = 600.0f;
			MoveComp->JumpZVelocity = 300.0f;
			break;
		}
	case ECharacterState::OnDamage:
		{
			MoveComp->MaxWalkSpeed = 1200.0f;
			MoveComp->JumpZVelocity = 300.0f;
			break;
		}
	}
	
}

void AACCharacter::ResetHoldInteract()
{
	if (CurrentHoldTarget == nullptr)
	{
		return;
	}

	AC_LOG(LogSW, Log, TEXT("%s Reset"), *CurrentHoldTarget->GetName());

	bIsHoldingInteract = false;
	CurrentHoldTarget = nullptr;
	CurrentHoldTime = 0.f;
	RequiredHoldTime = 0.f;
}

float AACCharacter::GetHoldProgress() const
{
	if (RequiredHoldTime <= 0.f)
	{
		return 0.f;
	}

	return FMath::Clamp(CurrentHoldTime / RequiredHoldTime, 0.f, 1.f);
}

void AACCharacter::MulticastPlayAttackMontage_Implementation()
{
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

EACCharacterType AACCharacter::GetCharacterType()
{
	return EACCharacterType::Citizen;
}