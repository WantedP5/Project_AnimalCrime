// Fill out your copyright notice in the Description page of Project Settings.


#include "ACCitizen.h"

#include "ACCharacter.h"
#include "BrainComponent.h"
#include "NavigationSystem.h"
#include "AI/ACCitizenAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/ACInteractableComponent.h"
#include "Component/ACMoneyComponent.h"
#include "Game/ACPlayerState.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AACCitizen::AACCitizen()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// AI 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AACCitizenAIController::StaticClass();

	GetCapsuleComponent()->InitCapsuleSize(35.f, 90.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("CitizenCollision"));

	// //캐릭터 무브먼트
	// auto Move = GetCharacterMovement();
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	// Move->JumpZVelocity = 600.f;
	// Move->AirControl = 0.3f;
	// Move->bOrientRotationToMovement = true;
	// Move->RotationRate = FRotator(0.f, 500.f, 0.f);

	//스켈레탈 메시
	USkeletalMeshComponent* MeshComp = GetMesh();
	MeshComp->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Body_010.SK_Body_010")));
	MeshComp->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	MeshComp->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimRef(TEXT("/Script/Engine.AnimBlueprint'/Game/Project/Character/ABP_ACPlayerHena.ABP_ACPlayerHena_C'"));
	if (AnimRef.Succeeded())
	{
		MeshComp->SetAnimInstanceClass(AnimRef.Class);
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DamagedMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project/Character/AM_OnDamaged.AM_OnDamaged'"));
	if (DamagedMontageRef.Succeeded())
	{
		DamagedMontage = DamagedMontageRef.Object;
	}
	
	
	GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
	
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	HeadMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	HeadMesh->SetupAttachment(RootComponent);
	HeadMesh->SetLeaderPoseComponent(MeshComp);
	HeadMesh->SetReceivesDecals(false);
	
	
	TopMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TopMesh"));
	TopMesh->SetupAttachment(GetMesh());
	TopMesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	TopMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	TopMesh->SetupAttachment(RootComponent);
	TopMesh->SetLeaderPoseComponent(MeshComp);
	TopMesh->SetReceivesDecals(false);

	BottomMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BottomMesh"));
	BottomMesh->SetupAttachment(GetMesh());
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

	// 인터랙션 컴포넌트
	InteractBoxComponent = CreateDefaultSubobject<UACInteractableComponent>(TEXT("InteractBoxComponent"));
	InteractBoxComponent->SetupAttachment(RootComponent);
	
	MoneyComp = CreateDefaultSubobject<UACMoneyComponent>(TEXT("MoneyComponent"));
}

// Called when the game starts or when spawned
void AACCitizen::BeginPlay()
{
	Super::BeginPlay();
	
	
	MoneyComp->InitMoneyComponent(EMoneyType::MoneyCitizenType);
}

// Called every frame
void AACCitizen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AACCitizen::PlayDamagedMontage(const FVector& Attack)
{
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector DirVector = (Attack - GetActorLocation()).GetSafeNormal2D();
	float FrontBackResult = ForwardVector.Dot(DirVector);
	float RightLeftResult = RightVector.Dot(DirVector);
	
	GetMesh()->GetAnimInstance()->Montage_Play(DamagedMontage);
	if (FrontBackResult >= 0)
	{
		if (RightLeftResult >= 0)
		{
			// Front
			if (FrontBackResult >= RightLeftResult)
			{
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("Front"), DamagedMontage);
				
				UE_LOG(LogTemp, Log, TEXT("Front - F:%f, R:%f"), FrontBackResult, RightLeftResult);
			}
			else
			{
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("Right"), DamagedMontage);

				UE_LOG(LogTemp, Log, TEXT("Right - F:%f, R:%f"), FrontBackResult, RightLeftResult);
			}
		}
		else
		{
			// Front
			if (FrontBackResult >= abs(RightLeftResult))
			{
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("Front"), DamagedMontage);

				UE_LOG(LogTemp, Log, TEXT("Front - F:%f, R:%f"), FrontBackResult, RightLeftResult);
			}
			else
			{
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("Left"), DamagedMontage);

				UE_LOG(LogTemp, Log, TEXT("Left - F:%f, R:%f"), FrontBackResult, RightLeftResult);
			}
		}
	}
	else
	{
		if (RightLeftResult >= 0)
		{
			// Front
			if (abs(FrontBackResult) >= abs(RightLeftResult))
			{
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("Back"), DamagedMontage);

				UE_LOG(LogTemp, Log, TEXT("Back - F:%f, R:%f"), FrontBackResult, RightLeftResult);
			}
			else
			{
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("Right"), DamagedMontage);

				UE_LOG(LogTemp, Log, TEXT("Right - F:%f, R:%f"), FrontBackResult, RightLeftResult);
			}
		}
		else
		{
			// Front
			if (abs(FrontBackResult) >= abs(RightLeftResult))
			{
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("Back"), DamagedMontage);

				UE_LOG(LogTemp, Log, TEXT("Back - F:%f, R:%f"), FrontBackResult, RightLeftResult);
			}
			else
			{
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("Left"), DamagedMontage);
				UE_LOG(LogTemp, Log, TEXT("Left - F:%f, R:%f"), FrontBackResult, RightLeftResult);
			}
		}
		
	}
}

FVector AACCitizen::GetNextPosition() const
{
	// 현재 액터의 위치
	FVector CurrentLocation = GetActorLocation();

	// Navigation System 가져오기
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys == nullptr)
	{
		return CurrentLocation;
	}

	// 반환할 NavMesh 랜덤 위치
	FNavLocation RandomLocation;
    
	// 반경 (원하는 만큼 조절)
	float Radius = 2000.0f;

	// NavMesh에서 랜덤 reachable 포인트 얻기
	NavSys->GetRandomReachablePointInRadius(CurrentLocation, Radius, RandomLocation);

	return RandomLocation.Location;
}

FVector AACCitizen::GetRunPosition(const FVector& Attack) const
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No navigation found1"));
		return FVector(0.0f, 0.0f, 0.0f);
	}
	
	FNavLocation Result;
	FVector MyLocation = GetActorLocation();
	FVector DirVector = (MyLocation - Attack).GetSafeNormal2D();
	FVector Desired = MyLocation + (DirVector * 3000.f);
	bool bFound = NavSys->ProjectPointToNavigation(Desired, Result, FVector(1000.f, 1000.f, 200.f));
	if (bFound == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("No navigation found2"));
		return FVector(0.0f, 0.0f, 0.0f);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("(%f, %f, %f)"), Result.Location.X, Result.Location.Y, Result.Location.Z);
	return Result.Location;
}

void AACCitizen::OnDamaged()
{
	DamagedFlag += 1;
	if (DamagedFlag > 1)
	{
		AAIController* AICon = GetController<AAIController>();
		AICon->GetBrainComponent()->StopLogic("HitAbort");

		AICon->GetBrainComponent()->StartLogic();
		
		
	}

}

void AACCitizen::OnArrive()
{
	DamagedFlag = 0;
}

float AACCitizen::GetLastHitTime() const
{
	return LastHitTime;
}

void AACCitizen::AttackHitCheck()
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
	
	// bool bHit = GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_GameTraceChannel2 | ECC_GameTraceChannel4, FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight), Params);
	
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel6);
	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel7);
	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel8);
	
	bool bHit = GetWorld()->SweepSingleByObjectType(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ObjectParams,
		FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),
		Params
	);
	
	// 디버그: 캡슐 그리기
	DrawDebugCapsule(GetWorld(), (Start + End) * 0.5f, CapsuleHalfHeight, CapsuleRadius, FRotationMatrix::MakeFromZ(End - Start).ToQuat(), bHit ? FColor::Red : FColor::Green, false, 1.0f);
	
	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName());
		UGameplayStatics::ApplyDamage(Hit.GetActor(),30.0f, GetController(),this, nullptr);
	}
}

void AACCitizen::MulticastOnPlayMontage_Implementation(const FVector& Attack)
{
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector DirVector = (Attack - GetActorLocation()).GetSafeNormal2D();
	float FrontBackResult = ForwardVector.Dot(DirVector);
	float RightLeftResult = RightVector.Dot(DirVector);
	
	GetMesh()->GetAnimInstance()->Montage_Play(DamagedMontage);
	if (FrontBackResult >= 0)
	{
		if (RightLeftResult >= 0)
		{
			// Front
			if (FrontBackResult >= RightLeftResult)
			{
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("Front"), DamagedMontage);
				// UE_LOG(LogTemp, Log, TEXT("Front - F:%f, R:%f"), FrontBackResult, RightLeftResult);
			}
			else
			{
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("Right"), DamagedMontage);
				// UE_LOG(LogTemp, Log, TEXT("Right - F:%f, R:%f"), FrontBackResult, RightLeftResult);
			}
		}
		else
		{
			// Front
			if (FrontBackResult >= abs(RightLeftResult))
			{
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("Front"), DamagedMontage);
				// UE_LOG(LogTemp, Log, TEXT("Front - F:%f, R:%f"), FrontBackResult, RightLeftResult);
			}
			else
			{
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("Left"), DamagedMontage);
				// UE_LOG(LogTemp, Log, TEXT("Left - F:%f, R:%f"), FrontBackResult, RightLeftResult);
			}
		}
	}
	else
	{
		if (RightLeftResult >= 0)
		{
			// Front
			if (abs(FrontBackResult) >= abs(RightLeftResult))
			{
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("Back"), DamagedMontage);
				// UE_LOG(LogTemp, Log, TEXT("Back - F:%f, R:%f"), FrontBackResult, RightLeftResult);
			}
			else
			{
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("Right"), DamagedMontage);
				// UE_LOG(LogTemp, Log, TEXT("Right - F:%f, R:%f"), FrontBackResult, RightLeftResult);
			}
		}
		else
		{
			// Front
			if (abs(FrontBackResult) >= abs(RightLeftResult))
			{
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("Back"), DamagedMontage);
				// UE_LOG(LogTemp, Log, TEXT("Back - F:%f, R:%f"), FrontBackResult, RightLeftResult);
			}
			else
			{
				GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("Left"), DamagedMontage);
				// UE_LOG(LogTemp, Log, TEXT("Left - F:%f, R:%f"), FrontBackResult, RightLeftResult);
			}
		}
		
	}
}


float AACCitizen::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float SuperDamage =  Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	UE_LOG(LogTemp, Log, TEXT("TakeDamage: %f"), SuperDamage);
	AACCitizenAIController* AIControler = Cast<AACCitizenAIController>(GetController());
	if (AIControler)
	{
		UBlackboardComponent* BBComp = AIControler->GetBlackboardComponent();
		if (BBComp)
		{
			// 의미가 있는 코드일까? 언제 전송되는데
			BBComp->SetValueAsBool("bDamage", true);
			
			FVector RunPosition = GetRunPosition(DamageCauser->GetActorLocation());
			BBComp->SetValueAsVector("RunPosition", RunPosition);
			LastHitTime = GetWorld()->GetTimeSeconds();
			BBComp->SetValueAsFloat("LastHitTime", LastHitTime);
			OnDamaged();
			//PlayDamagedMontage(DamageCauser->GetActorLocation());
			MulticastOnPlayMontage(DamageCauser->GetActorLocation());
		}
		
		int32 Money = FMath::RandRange(1, 100);
		int32 Result = MoneyComp->LoseMoney(Money);
		AACCharacter* Test = Cast<AACCharacter>(DamageCauser);
		if (Test == nullptr)
		{
			return SuperDamage;
		}
		Test->MoneyComp->EarnMoney(Result);
		UE_LOG(LogTemp, Warning, TEXT("Earn Money: %d Cur Money: %d"), Result, Test->MoneyComp->GetMoney());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Oh No"));
	}
	return SuperDamage;
}

bool AACCitizen::CanInteract(AACCharacter* ACPlayer)
{
	return true;
}

void AACCitizen::OnInteract(AACCharacter* ACPlayer)
{
	ShowInteractDebug(ACPlayer, GetName());
}

