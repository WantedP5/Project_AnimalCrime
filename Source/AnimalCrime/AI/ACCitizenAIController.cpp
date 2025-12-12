// Fill out your copyright notice in the Description page of Project Settings.


#include "ACCitizenAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


// Sets default values
AACCitizenAIController::AACCitizenAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBWAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Project/AI/BB_Citizen.BB_Citizen'"));
	ensureAlways(BBWAssetRef.Object);

	// Blackboard 에셋 연결
	if (BBWAssetRef.Object)
	{
		BBAsset = BBWAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTWAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Project/AI/BT_Citizen.BT_Citizen'"));
	ensureAlways(BTWAssetRef.Object);

	// Behavior Tree 에셋 연결
	if (BTWAssetRef.Object)
	{
		BTAsset = BTWAssetRef.Object;
		BTAsset->BlackboardAsset = BBAsset;
	}
}

// Called when the game starts or when spawned
void AACCitizenAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AACCitizenAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AACCitizenAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	UE_LOG(LogTemp, Log, TEXT("OnPossess Name: %s"), *InPawn->GetName());
	
	RunAI();
}

void AACCitizenAIController::OnUnPossess()
{
	Super::OnUnPossess();
	
	StopAI();
}

void AACCitizenAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();

	bool UseResult = UseBlackboard(BBAsset, BlackboardPtr);
	if (UseResult == false)
	{
		ensureAlways(UseResult);
		return;
	}

	APawn* PawnPtr = GetPawn();
	BlackboardPtr->SetValueAsVector(TEXT("Position"), FVector(-1200, 3600, 0));

	bool RunResult = RunBehaviorTree(BTAsset);
	if (RunResult == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AVMBossAIController::RunAI] RunBehaviorTree를 실패하였습니다."));
		return;
	}
}

void AACCitizenAIController::StopAI()
{
}
