// Fill out your copyright notice in the Description page of Project Settings.


#include "ACCitizenAIController.h"

#include "AnimalCrime.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/ACCitizen.h"


#pragma region 생성자

AACCitizenAIController::AACCitizenAIController()
{
	// AI Controller의 Tick 제거
	PrimaryActorTick.bCanEverTick = false;
	
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackBoardAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Project/AI/BB_Citizen.BB_Citizen'"));
	// Blackboard 에셋 연결
	if (BlackBoardAssetRef.Succeeded() == false)
	{
		UE_LOG(LogHY, Error, TEXT("Begin"));
	}
	BBAsset = BlackBoardAssetRef.Object;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTWAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Project/AI/BT_Citizen.BT_Citizen'"));
	ensureAlways(BTWAssetRef.Object);

	// Behavior Tree 에셋 연결
	if (BTWAssetRef.Object)
	{
		BTAsset = BTWAssetRef.Object;
		BTAsset->BlackboardAsset = BBAsset;
	}
}

#pragma endregion

#pragma region 엔진 제공 함수

void AACCitizenAIController::PostInitializeComponents()
{
	AC_LOG(LogHY, Error, TEXT("Begin"));
	Super::PostInitializeComponents();
	AC_LOG(LogHY, Error, TEXT("End"));
}

void AACCitizenAIController::PostNetInit()
{
	AC_LOG(LogHY, Error, TEXT("Begin"));
	Super::PostNetInit();
	AC_LOG(LogHY, Error, TEXT("End"));
}

void AACCitizenAIController::BeginPlay()
{
	AC_LOG(LogHY, Error, TEXT("Begin"));
	Super::BeginPlay();
	AC_LOG(LogHY, Error, TEXT("End"));
}

void AACCitizenAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AACCitizenAIController::OnPossess(APawn* InPawn)
{
	AC_LOG(LogHY, Error, TEXT("Begin"));
	Super::OnPossess(InPawn);
	
	UE_LOG(LogTemp, Log, TEXT("OnPossess Name: %s"), *InPawn->GetName());
	
	RunAI();
	AC_LOG(LogHY, Error, TEXT("End"));
}

void AACCitizenAIController::OnUnPossess()
{
	Super::OnUnPossess();
	
	StopAI();
}

#pragma endregion

void AACCitizenAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();

	bool UseResult = UseBlackboard(BBAsset, BlackboardPtr);
	if (UseResult == false)
	{
		ensureAlways(UseResult);
		return;
	}

	bool RunResult = RunBehaviorTree(BTAsset);
	if (RunResult == false)
	{
		AC_LOG(LogHY, Error, TEXT("행동트리 동작 불가"));
		return;
	}
	
	UBrainComponent* LocalBrainComp = BrainComponent;
	if (LocalBrainComp == nullptr)
	{
		AC_LOG(LogHY, Error, TEXT("LocalBrainComp 존재하지 않아서 Tick 설정 불가."));
		return;
	}
	LocalBrainComp->SetComponentTickInterval(1.0f);
}

void AACCitizenAIController::StopAI()
{
	UBrainComponent* LocalBrainComp = GetBrainComponent();
	if (LocalBrainComp == nullptr)
	{
		AC_LOG(LogHY, Error, TEXT("LocalBrainComp 존재하지 않아서 StopAI 불가."));
		return;
	}
	
	LocalBrainComp->StopLogic(TEXT("AI Stopped by StopAI function"));
}

bool AACCitizenAIController::GetNextPosition(FVector& InOutPosition) const
{
	APawn* PossessedPawn = GetPawn();
	if (PossessedPawn == nullptr)
	{
		return false;
	}
	
	const AACCitizen* CitizenPawn = Cast<AACCitizen>(PossessedPawn);
	if (CitizenPawn == nullptr)
	{
		return false;
	}
	
	// Citizen의 함수를 통해 가져옴.
	InOutPosition = CitizenPawn->GetNextPosition();
	if (InOutPosition == FVector::Zero())
	{
		
	}
	
	return true;
}
