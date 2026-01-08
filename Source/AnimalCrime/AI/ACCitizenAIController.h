// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "ACCitizenAIController.generated.h"

UCLASS()
class ANIMALCRIME_API AACCitizenAIController : public AAIController
{
	GENERATED_BODY()

#pragma region 특수 맴버 함수
public:
	AACCitizenAIController();
#pragma endregion

#pragma region 엔진 제공 함수
public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void PostNetInit() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
#pragma endregion
	
#pragma region AI 작동 로직
public:
	void RunAI();
	void StopAI();
#pragma endregion
	
#pragma region AI Task 로직
public:
	bool GetNextPosition(FVector& InOutPosition) const;
#pragma endregion
	
#pragma region 맴버 변수
public:
	
protected:
	// Blackboard 애셋.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBlackboardData> BBAsset;

	// Behaviour Tree 애셋.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTree> BTAsset;

#pragma region 캐시용 변수
	// UPROPERTY(EditAnywhere, Category = AI)
	// TObjectPtr<class UBehaviorTreeComponent> BTComp;
#pragma endregion
	
private:
#pragma endregion
};
