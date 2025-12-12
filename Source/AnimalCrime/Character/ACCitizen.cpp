// Fill out your copyright notice in the Description page of Project Settings.


#include "ACCitizen.h"

#include "NavigationSystem.h"
#include "AI/ACCitizenAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AACCitizen::AACCitizen()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// AI 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AACCitizenAIController::StaticClass();

	GetCapsuleComponent()->InitCapsuleSize(35.f, 90.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

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
}

// Called when the game starts or when spawned
void AACCitizen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AACCitizen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	float Radius = 500.0f;

	// NavMesh에서 랜덤 reachable 포인트 얻기
	NavSys->GetRandomReachablePointInRadius(CurrentLocation, Radius, RandomLocation);

	return RandomLocation.Location;
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
			BBComp->SetValueAsBool("bDamage", true);
			FNavLocation Result;
			UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
			UE_LOG(LogTemp, Log, TEXT("Welcome1"));
			if (NavSys)
			{
				FVector Cur = GetActorLocation();
				FVector Dir = (Cur - DamageCauser->GetActorLocation()).GetSafeNormal2D();
				FVector Desired = Cur + (Dir * 2000.f);
				FVector ForwardVector = Dir;
				bool bFound = NavSys->ProjectPointToNavigation(Desired, Result, FVector(200.f, 200.f, 200.f));
				UE_LOG(LogTemp, Log, TEXT("Welcome2 Actor Location(%f, %f, %f) Forward(%f, %f, %f)"), Cur.X, Cur.Y, Cur.Z, ForwardVector.X, ForwardVector.Y, ForwardVector.Z);
				if (bFound)
				{
					FVector FleeLocation = Result.Location;
					BBComp->SetValueAsVector("Position", FleeLocation);
					UE_LOG(LogTemp, Log, TEXT("Welcome3 (%f, %f, %f) dist:%f"), FleeLocation.X, FleeLocation.Y, FleeLocation.Z, FVector::Dist(Cur, FleeLocation));
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Oh No"));
	}
	return SuperDamage;
}

