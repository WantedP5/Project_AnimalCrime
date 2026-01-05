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
#include "ACCharacterAnimInstance.h"
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

#include "Game/ACPlayerState.h"

#include "Sound/SoundBase.h"
#include "Net/VoiceConfig.h"
#include "EngineUtils.h"

AACCharacter::AACCharacter()
{
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(35.f, 90.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	//캐릭터 무브먼트
	auto Move = GetCharacterMovement();
	Move->MaxWalkSpeed = 300.f;
	Move->JumpZVelocity = 500.f;
	Move->AirControl = 0.3f;
	Move->bOrientRotationToMovement = true;
	Move->RotationRate = FRotator(0.f, 500.f, 0.f);

	//스켈레탈 메시
	USkeletalMeshComponent* MeshComp = GetMesh();
	MeshComp->SetSkeletalMesh(
		LoadObject<USkeletalMesh>(
			nullptr, TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Body_010.SK_Body_010")));
	MeshComp->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	MeshComp->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	MeshComp->SetReceivesDecals(false);

	static ConstructorHelpers::FClassFinder<UACCharacterAnimInstance> AnimRef(
		TEXT("/Script/Engine.AnimBlueprint'/Game/Project/Character/ABP_ACPlayer.ABP_ACPlayer_C'"));
	if (AnimRef.Succeeded())
	{
		MeshComp->SetAnimInstanceClass(AnimRef.Class);
	}

	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HeadMeshRef(
		TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Hat_049.SK_Hat_049"));
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
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FaceMeshRef(
		TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Male_emotion_usual_001.SK_Male_emotion_usual_001"));
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
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TopMeshRef(
		TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Outerwear_036.SK_Outerwear_036"));
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
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BottomMeshRef(
		TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Pants_014.SK_Pants_014"));
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
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ShoesMeshRef(
		TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Shoe_Slippers_005.SK_Shoe_Slippers_005"));
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
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FaceAccMeshRef(
		TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Moustache_002.SK_Moustache_002"));
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

	// 애니메이션 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeleeMontageRef(
		TEXT("/Game/Project/Character/AM_Melee.AM_Melee"));
	if (MeleeMontageRef.Succeeded())
	{
		MeleeMontage = MeleeMontageRef.Object;
	}

	// ShopComponent 생성
	ShopComponent = CreateDefaultSubobject<UACShopComponent>(TEXT("ShopComponent"));

	GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
	MoneyComp = CreateDefaultSubobject<UACMoneyComponent>(TEXT("MoneyComponent"));


	bReplicates = true;

	// 방망이 휘두르기 사운드 로드
	static ConstructorHelpers::FObjectFinder<USoundBase> BatSwingSoundRef(
		TEXT("/Game/Project/SFX/422506__nightflame__swinging-staff-whoosh-strong-01")
	);
	if (BatSwingSoundRef.Succeeded())
	{
		BatSwingSound = BatSwingSoundRef.Object;
	}

	// VOIPTalker 생성
	VOIPTalker = CreateDefaultSubobject<UVOIPTalker>(TEXT("VOIPTalker"));

	static ConstructorHelpers::FObjectFinder<USoundAttenuation> AttenuationRef(TEXT("/Game/Project/Sound/SA_VOIP.SA_VOIP"));
	if (AttenuationRef.Succeeded())
	{
		VoiceAttenuation = AttenuationRef.Object;
	}
}


void AACCharacter::BeginPlay()
{
	Super::BeginPlay();


	CharacterState = ECharacterState::Free;

	// @Todo 변경 필요. Mafia와 Police 구분이 안감.
	// Police와 Mafia는 각자의 BeginPlay에서 초기화
	// ※ 얘도 확인 했으면 지워주세요
	//MoneyComp->InitMoneyComponent(EMoneyType::MoneyMafiaType);

	// VOIPTalker 초기화 (다른 플레이어 캐릭터에만 적용)
	if (IsLocallyControlled())
	{
		return;
	}

	TryRegisterVOIPTalker();

}

void AACCharacter::TryRegisterVOIPTalker()
{
	if (VOIPTalker == nullptr)
	{
		AC_LOG(LogSY, Error, TEXT("VOIPTalker is nullptr in %s"), *GetName());
		return;
	}

	APlayerState* PS = GetPlayerState();
	if (PS == nullptr)
	{
		// PlayerState가 아직 없으면 타이머로 재시도
		GetWorld()->GetTimerManager().SetTimer(
			VOIPTalkerTimerHandle,
			this,
			&AACCharacter::TryRegisterVOIPTalker,
			0.1f,
			false
		);
		return;
	}

	// 타이머 정리
	GetWorld()->GetTimerManager().ClearTimer(VOIPTalkerTimerHandle);

	if (VoiceAttenuation == nullptr)
	{
		AC_LOG(LogSY, Warning, TEXT("VoiceAttenuation is nullptr in %s"), *GetName());
	}

	VOIPTalker->Settings.ComponentToAttachTo = GetRootComponent();

	// Register 전에 무전기 상태에 따라 Attenuation 결정
	bool bDisableAttenuation = false;
	APlayerController* LocalPC = GetWorld()->GetFirstPlayerController();
	if (LocalPC != nullptr)
	{
		AC_LOG(LogSY, Warning, TEXT("Local PlayerController is nullptr"));

		AACCharacter* LocalChar = Cast<AACCharacter>(LocalPC->GetPawn());
		if (LocalChar != nullptr)
		{
			if (VoiceGroup != EVoiceGroup::None && VoiceGroup == LocalChar->VoiceGroup)
			{
				// 양쪽 다 무전기 있으면서 같은 무전기 그룹이면 Attenuation 제거
				bDisableAttenuation = true;
				AC_LOG(LogSY, Log, TEXT("VOIPTalker Attenuation disabled for %s (Radio mode)"), *GetName());
			}
			else
			{
				AC_LOG(LogSY, Log, TEXT("VOIPTalker Attenuation enabled for %s (Normal mode)"), *GetName());
			}
		}
		else
		{
			AC_LOG(LogSY, Warning, TEXT("Local PlayerCharacter is nullptr"));
		}
	}
	else
	{
		AC_LOG(LogSY, Warning, TEXT("Local PlayerController is nullptr"));
	}

	VOIPTalker->Settings.AttenuationSettings = bDisableAttenuation ? nullptr : VoiceAttenuation;
	VOIPTalker->RegisterWithPlayerState(PS);

	AC_LOG(LogSY, Log, TEXT("VOIPTalker registered for %s, Attenuation: %s"), *GetName(), VOIPTalker->Settings.AttenuationSettings ? TEXT("Enabled") : TEXT("Disabled"));
}

void AACCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AACCharacter, CharacterState);
	DOREPLIFETIME(AACCharacter, HeadMeshReal);
	DOREPLIFETIME(AACCharacter, FaceMeshReal);
	DOREPLIFETIME(AACCharacter, TopMeshReal);
	DOREPLIFETIME(AACCharacter, BottomMeshReal);
	DOREPLIFETIME(AACCharacter, ShoesMeshReal);
	DOREPLIFETIME(AACCharacter, FaceAccMeshReal);
	//DOREPLIFETIME(AACCharacter, bHasRadio);
	DOREPLIFETIME(AACCharacter, VoiceGroup);
}

void AACCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AC_LOG(LogHY, Warning, TEXT("AACCharacter::EndPlay %s"), *GetName());

	// VOIPTalker 정리 (맵 이동 시 VoipListenerSynthComponent 정리를 위해)
	// DestroyComponent() 사용 금지 - 오디오 스레드가 비동기로 참조 중일 수 있음
	//if (VOIPTalker != nullptr)
	//{
	//	APlayerState* PS = GetPlayerState();
	//	if (PS != nullptr)
	//	{
	//		UVOIPStatics::ResetPlayerVoiceTalker(PS);
	//	}
	//	// DestroyComponent() 대신 Deactivate만 하고 GC에 맡김
	//	VOIPTalker->Deactivate();
	//	VOIPTalker = nullptr;
	//}

	// VOIPTalker 정리 (fallback). 맵 이동시에는 미리 정리함. 이건 강제 종료 시 정리용.
	CleanupVOIPTalker();

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	Super::EndPlay(EndPlayReason);
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
	if (SortNearInteractables() == false)
	{
		return;
	}

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

			// Server RPC로 대상 움직임 멈춤 (ACharacter 사용 - 시민도 포함)
			ACharacter* TargetChar = Cast<ACharacter>(CurrentHoldTarget);
			if (TargetChar != nullptr)
			{
				ServerFreezeCharacter(TargetChar, true);
			}


			AACMainPlayerController* PC = Cast<AACMainPlayerController>(GetController());
			if (PC == nullptr)
			{
				return;
			}

			// 홀드 UI 표시
			//UE_LOG(LogSW, Log, TEXT("Show start"));
			PC->ShowInteractProgress(CurrentHoldTarget->GetName());
		}

		break;
	}
}

void AACCharacter::InteractHolding(const float DeltaTime)
{
	if (bIsHoldingInteract == false)
	{
		//ResetHoldInteract();
		return;
	}
	//todo: isvalid?
	if (CurrentHoldTarget == nullptr)
	{
		InteractReleased();
		return;
	}
	if (NearInteractables.Contains(CurrentHoldTarget) == false)
	{
		InteractReleased();
		return;
	}


	// 시간 누적
	CurrentHoldTime += DeltaTime;
	//AC_LOG(LogSW, Log, TEXT("%s Holding at %f"), *CurrentHoldTarget->GetName(), CurrentHoldTime);

	// UI 업데이트
	AACMainPlayerController* PC = Cast<AACMainPlayerController>(GetController());
	if (PC != nullptr)
	{
		PC->UpdateInteractProgress(GetHoldProgress());
	}

	// 홀드 시간 완료 시 상호작용 실행
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
	if (PC != nullptr)
	{
		//UE_LOG(LogSW, Log, TEXT("Hide start"));
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
		return;
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

void AACCharacter::ServerDash_Implementation()
{
	if (GetWorldTimerManager().IsTimerActive(DashTimerHandle) == true)
	{
		AC_LOG(LogHY, Error, TEXT("Dash Timer is already active"));
		return;
	}

	FVector ForwardDir = GetActorForwardVector();

	FVector LaunchVel = ForwardDir * 1000 + FVector(0.f, 0.f, 500.f);
	LaunchCharacter(LaunchVel, true, false);
	FTimerDelegate TimerDelegate;
	bDashCoolDown = false;
	TimerDelegate.BindUObject(this, &AACCharacter::ResetDashFlag);
	GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, TimerDelegate, 10, false);
}

void AACCharacter::ResetDashFlag()
{
	bDashCoolDown = true;
}

void AACCharacter::Sprint(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		if (SprintGauge > 0)
		{
			ServerSprintStart();
		}
	}
	else
	{
		ServerSprintEnd();
	}
}

void AACCharacter::ServerSprintStart_Implementation()
{
	if (bSprint == true)
	{
		return;
	}

	// Delete Guage Up 
	GetWorld()->GetTimerManager().ClearTimer(SprintGaugeUpTimerHandle);

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &AACCharacter::GaugeDown);

	GetWorld()->GetTimerManager().SetTimer(SprintGaugeDownTimerHandle,
		TimerDelegate,
		1, true);


	bSprint = true;
	// AC_LOG(LogHY, Error, TEXT("ServerSprintStart_Implementation %d"), bSprint);
	OnRep_Sprint();
}

void AACCharacter::ServerSprintEnd_Implementation()
{
	// if (bSprint == false)
	// {
	// 	return;
	// }
	// Delete Guage Down 
	GetWorld()->GetTimerManager().ClearTimer(SprintGaugeDownTimerHandle);

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &AACCharacter::GaugeUp);

	GetWorld()->GetTimerManager().SetTimer(SprintGaugeUpTimerHandle,
		TimerDelegate,
		1, true);


	bSprint = false;
	OnRep_Sprint();
}

// void AACCharacter::ServerSprint_Implementation()
// {
// 	// // bSprint = !bSprint;
// 	// if (bSprint == false)
// 	// {
// 	// 	bSprint = true;
// 	// }
// 	// else
// 	// {
// 	// 	bSprint = false;
// 	// }
// 	// OnRep_Sprint();
// }

void AACCharacter::ResetSprint()
{
	bSprint = false;
}

void AACCharacter::OnRep_Sprint()
{
	if (bSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = 1200;
		// AC_LOG(LogHY, Warning, TEXT("Sprint is active %f"), GetCharacterMovement()->MaxWalkSpeed);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 600;
		// AC_LOG(LogHY, Warning, TEXT("Sprint is deactive %f"), GetCharacterMovement()->MaxWalkSpeed);
	}
}

void AACCharacter::GaugeUp()
{
	SprintGauge += 1;
	// AC_LOG(LogHY, Warning, TEXT("Gauge Up: %d"), SprintGauge);
	if (SprintGauge > 10)
	{
		SprintGauge = 10;
		// Todo
	}
}

void AACCharacter::GaugeDown()
{
	SprintGauge -= 1;
	// AC_LOG(LogHY, Warning, TEXT("Gauge Down: %d"), SprintGauge);
	if (SprintGauge <= 0)
	{
		GetWorldTimerManager().ClearTimer(SprintGaugeDownTimerHandle);
		bSprint = false;
		OnRep_Sprint();
	}
}

void AACCharacter::Dash(const FInputActionValue& Value)
{
	bool Flag = Value.Get<bool>();
	AC_LOG(LogHY, Error, TEXT("Try Dash!! %d"), Flag);

	if (Flag == false)
	{
		AC_LOG(LogHY, Error, TEXT("Dash Input False"));
		return;
	}

	// Client가 Server에게 대쉬 요청
	ServerDash();
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

void AACCharacter::SetCarryState(bool bPlay)
{
	if (HasAuthority())
	{
		Multicast_SetCarryState(bPlay);
	}
	else
	{
		Server_SetCarryState(bPlay);
	}
}

void AACCharacter::Server_SetCarryState_Implementation(bool bPlay)
{
	Multicast_SetCarryState(bPlay);
}

void AACCharacter::Multicast_SetCarryState_Implementation(bool bPlay)
{
	UACCharacterAnimInstance* Anim = Cast<UACCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (Anim == nullptr)
	{
		return;
	}

	Anim->SetIsCarrying(bPlay);
}

bool AACCharacter::CanInteract(AACCharacter* ACPlayer)
{
	if (ACPlayer == nullptr)
	{
		return false;
	}

	// 시민과 시민은 상호작용 가능하다?
	return true;
}

void AACCharacter::OnInteract(AACCharacter* ACPlayer)
{
	//ShowInteractDebug(ACPlayer, GetName());
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
		//AC_LOG(LogSW, Log, TEXT("NO NEED TO SORT!!"));
		return true;
	}

	const FVector PlayerLocation = GetActorLocation();
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
	//if (IsValid(this) == false)
	//{
	//	return;
	//}		

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (IsValid(this) == false)
	{
		AC_LOG(LogHY, Error, TEXT("this is Invalid"));
		return;
	}

	if (MoveComp == nullptr)
	{
		AC_LOG(LogHY, Error, TEXT("MoveComp is nullptr"));
		return;
	}

	AC_LOG(LogHY, Error, TEXT("CharacterType:%d name:%s"), CharacterState, *GetName());

	switch (CharacterState)
	{
	case ECharacterState::Interact:
	{
		// MoveComp->SetMovementMode(MOVE_None);
		break;
	}
	case ECharacterState::Stun:
	{
		MoveComp->MaxWalkSpeed = 10.f;
		MoveComp->JumpZVelocity = 0.f;
		break;
	}
	case ECharacterState::Free:
	{
		// MoveComp->SetMovementMode(MOVE_Walking);
		if (GetCharacterType() == EACCharacterType::Police)
		{
			MoveComp->MaxWalkSpeed = 500.0f; // 경찰
		}
		else
		{
			MoveComp->MaxWalkSpeed = 300.0f; // 마피아/시민
		}
		MoveComp->JumpZVelocity = 500.0f;
		break;
	}
	case ECharacterState::OnDamage:
	{
		MoveComp->MaxWalkSpeed = 600.0f;
		break;
	}
	}
}

void AACCharacter::SetCharacterState(ECharacterState InCharacterState)
{
	CharacterState = InCharacterState;
}

void AACCharacter::ResetHoldInteract()
{
	if (CurrentHoldTarget == nullptr)
	{
		return;
	}

	// Server RPC로 대상 움직임 재개 (ACharacter 사용 - 시민도 포함)
	ACharacter* TargetChar = Cast<ACharacter>(CurrentHoldTarget);
	if (TargetChar != nullptr)
	{
		ServerFreezeCharacter(TargetChar, false);
	}

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

void AACCharacter::CleanupVOIPTalker()
{
	if (VOIPTalker == nullptr)
	{
		return;  // 이미 정리됨
	}

	AC_LOG(LogSY, Log, TEXT("CleanupVOIPTalker - Character: %s"), *GetName());

	// PlayerState를 통해 VoiceTalker 리셋
	APlayerState* PS = GetPlayerState();
	if (PS == nullptr)
	{
		AC_LOG(LogSY, Warning, TEXT("PlayerState is nullptr"));
		return;
	}

	UVOIPStatics::ResetPlayerVoiceTalker(PS);

	// Deactivate만 하고 GC에 맡김 (오디오 스레드 안전)
	VOIPTalker->Deactivate();
	VOIPTalker = nullptr;

	AC_LOG(LogSY, Log, TEXT("  - VOIPTalker deactivated"));
}

void AACCharacter::MulticastPlayAttackMontage_Implementation()
{
	if (MeleeMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MeleeMontage, 2.0f);
	}

	// 로컬 클라이언트 + 무기 장착 시에만 사운드 재생
	if (IsLocallyControlled() && BatSwingSound && ShopComponent)
	{
		UACItemData* EquippedWeapon = ShopComponent->EquippedWeapon;

		// 무기를 들고 있는지 확인
		if (EquippedWeapon != nullptr && EquippedWeapon->ItemType == EItemType::Weapon)
		{
			UGameplayStatics::PlaySound2D(this, BatSwingSound);
		}
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

void AACCharacter::ServerSetTargetState_Implementation(AACCharacter* Target, ECharacterState NewState)
{
	if (Target == nullptr)
	{
		return;
	}
	Target->SetCharacterState(NewState);
}

void AACCharacter::ServerFreezeCharacter_Implementation(ACharacter* Target, bool bFreeze)
{
	if (Target == nullptr)
	{
		return;
	}

	UCharacterMovementComponent* MoveComp = Target->GetCharacterMovement();
	if (MoveComp == nullptr)
	{
		return;
	}

	if (bFreeze)
	{
		MoveComp->SetMovementMode(MOVE_None);
	}
	else
	{
		MoveComp->SetMovementMode(MOVE_Walking);
	}
}

EACCharacterType AACCharacter::GetCharacterType()
{
	return EACCharacterType::Citizen;
}

void AACCharacter::OnRep_HeadMesh() const
{
	if (HeadMesh)
	{
		UpdateHeadMesh();
	}
}

void AACCharacter::OnRep_FaceMesh() const
{
	if (FaceMesh)
	{
		UpdateFaceMesh();
	}
}

void AACCharacter::OnRep_TopMesh() const
{
	if (TopMesh)
	{
		UpdateTopMesh();
	}
}

void AACCharacter::OnRep_BottomMesh() const
{
	if (BottomMesh)
	{
		UpdateBottomMesh();
	}
}

void AACCharacter::OnRep_ShoesMesh() const
{
	if (ShoesMesh)
	{
		UpdateShoesMesh();
	}
}

void AACCharacter::OnRep_FaceAccMesh() const
{
	if (FaceAccMesh)
	{
		UpdateFaceAccMesh();
	}
}

//void AACCharacter::SetHasRadio(bool bNewHasRadio)
//{
//	if (bHasRadio == bNewHasRadio)
//	{
//		return;
//	}
//
//	bHasRadio = bNewHasRadio;
//
//	// 클라이언트들 → 자동으로 OnRep 호출됨
//	// 서버(Listen Server 호스트) → 수동 호출 필요
//	if (HasAuthority() == true)
//	{
//		OnRep_HasRadio();
//	}
//}

//void AACCharacter::OnRep_HasRadio()
//{
//	// 로컬 플레이어 캐릭터가 무전기 상태 변경 시 모든 다른 캐릭터의 VOIP 설정 업데이트
//	if (IsLocallyControlled() == true)
//	{
//		UpdateRadioVoiceSettings();
//	}
//	else
//	{
//		// 다른 캐릭터의 무전기 상태가 변경됨 → 로컬 플레이어가 무전기 있으면 해당 캐릭터 VOIP 업데이트
//		APlayerController* LocalPC = GetWorld()->GetFirstPlayerController();
//		if (LocalPC == nullptr)
//		{
//			return;
//		}
//
//		AACCharacter* LocalChar = Cast<AACCharacter>(LocalPC->GetPawn());
//		if (LocalChar != nullptr && LocalChar->GetHasRadio() == true)
//		{
//			// 나(로컬)도 무전기 있고, 상대도 무전기 있으면 Attenuation 제거
//			SetVOIPAttenuation(!bHasRadio);
//		}
//	}
//}

void AACCharacter::OnRep_VoiceGroup()
{
	// 로컬 플레이어 캐릭터가 무전기 상태 변경 시 모든 다른 캐릭터의 VOIP 설정 업데이트
	if (IsLocallyControlled() == true)
	{
		UpdateRadioVoiceSettings();
	}
	else
	{
		// 다른 캐릭터의 무전기 상태가 변경됨 → 로컬 플레이어가 무전기 있으면 해당 캐릭터 VOIP 업데이트
		APlayerController* LocalPC = GetWorld()->GetFirstPlayerController();
		if (LocalPC == nullptr)
		{
			return;
		}

		AACCharacter* LocalChar = Cast<AACCharacter>(LocalPC->GetPawn());
		if (LocalChar == nullptr)
		{
			return;
		}

		if (VoiceGroup != EVoiceGroup::None && LocalChar->VoiceGroup == VoiceGroup)
		{
			// 나(로컬)도 무전기 있고, 상대도 무전기 있으면서 같은 무전기 그룹이면 Attenuation 제거
			SetVOIPAttenuation(false);
		}
	}
}

void AACCharacter::SetVoiceGroupp(EVoiceGroup NewVoiceGroup)
{
	if (VoiceGroup == NewVoiceGroup)
	{
		return;
	}

	VoiceGroup = NewVoiceGroup;

	// 클라이언트들 → 자동으로 OnRep 호출됨
	// 서버(Listen Server 호스트) → 수동 호출 필요
	if (HasAuthority() == true)
	{
		OnRep_VoiceGroup();
	}
}

void AACCharacter::UpdateRadioVoiceSettings()
{
	// 로컬 플레이어만 호출
	if (IsLocallyControlled() == false)
	{
		return;
	}

	// 모든 ACCharacter를 순회하며 VOIP 설정 업데이트
	for (TActorIterator<AACCharacter> It(GetWorld()); It; ++It)
	{
		AACCharacter* OtherChar = *It;
		if (OtherChar == this)
		{
			continue;
		}

		// 나도 무전기 있고 상대도 무전기 있으면서 같은 그룹이면 → Attenuation 제거
		if (VoiceGroup != EVoiceGroup::None && VoiceGroup == OtherChar->VoiceGroup)
		{
			OtherChar->SetVOIPAttenuation(false);
		}
		else
		{
			OtherChar->SetVOIPAttenuation(true);
		}
		//bool bBothHaveRadio = bHasRadio && OtherChar->GetHasRadio();
		//OtherChar->SetVOIPAttenuation(!bBothHaveRadio);
	}
}

void AACCharacter::SetVOIPAttenuation(bool bUseAttenuation)
{
	if (VOIPTalker == nullptr)
	{
		return;
	}

	if (bUseAttenuation == true)
	{
		VOIPTalker->Settings.AttenuationSettings = VoiceAttenuation;
	}
	else
	{
		VOIPTalker->Settings.AttenuationSettings = nullptr;
	}
}
