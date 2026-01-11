// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/ACGameEnums.h"
#include "UObject/Object.h"
#include "ACGameRuleManager.generated.h"

class AACMainGameMode;

UENUM()
enum class EPoliceAction : uint8
{
	None UMETA(DisplayName = "None"),

	DestroyObject UMETA(DisplayName = "오브젝트를 파괴하다"),
	AttackCivilian UMETA(DisplayName = "시민을 공격하다"),
	// Arrest UMETA(DisplayName = "체포하다"),
	// CheckID UMETA(DisplayName = "신분증을 검사하다"),
	//
	// // --- 추가 행동들 ---
	// Patrol UMETA(DisplayName = "순찰하다"),
	// ReportSuspicious UMETA(DisplayName = "의심되는 행동을 보고하다"),
	// CallBackup UMETA(DisplayName = "지원 요청하다"),
	// RescueCivilian UMETA(DisplayName = "시민을 구조하다"),
	// UseTaser UMETA(DisplayName = "테이저를 사용하다"),
	// InteractTerminal UMETA(DisplayName = "터미널을 조작하다"),
	// InspectObject UMETA(DisplayName = "물체를 조사하다"),
	// BlockEscapeRoute UMETA(DisplayName = "도주 경로를 차단하다"),
	// SecureArea UMETA(DisplayName = "지역을 확보하다")
};

UENUM()
enum class EMafiaAction : uint8
{
	None UMETA(DisplayName = "None"),

	DestroyObject UMETA(DisplayName = "오브젝트를 파괴하다"),
	AttackCivilian UMETA(DisplayName = "시민을 공격하다"),
	ClearQuest UMETA(DisplayName = "퀘스트를 완료하다"),
	//
	// // --- 추가 행동들 ---
	// Patrol UMETA(DisplayName = "순찰하다"),
	// ReportSuspicious UMETA(DisplayName = "의심되는 행동을 보고하다"),
	// CallBackup UMETA(DisplayName = "지원 요청하다"),
	// RescueCivilian UMETA(DisplayName = "시민을 구조하다"),
	// UseTaser UMETA(DisplayName = "테이저를 사용하다"),
	// InteractTerminal UMETA(DisplayName = "터미널을 조작하다"),
	// InspectObject UMETA(DisplayName = "물체를 조사하다"),
	// BlockEscapeRoute UMETA(DisplayName = "도주 경로를 차단하다"),
	// SecureArea UMETA(DisplayName = "지역을 확보하다")
};

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACGameRuleManager : public UObject
{
	GENERATED_BODY()
#pragma region 특수 맴버 함수
public:
	UACGameRuleManager();
#pragma endregion
	
public:
	void Init(AACMainGameMode* InGameMode);
	
	AACMainGameMode* GetOwner() const;
	
	float GetScoreGauge() const;
	
public:
	UFUNCTION(BlueprintCallable)
	void RewardPoliceForAction(EPoliceAction Action, float InScore);
	
	UFUNCTION(BlueprintCallable)
	void RewardMafiaForAction(EMafiaAction Action, float InScore);
	
public:
	void HandleVictory();

private:
	FTimerHandle TimerHandle_NextMap;
	void LoadNextMap();
	
private:
	UFUNCTION(BlueprintCallable)
	void OnObjectDestroyed(float InScore);
	
	UFUNCTION(BlueprintCallable)
	void OnAttackCitizen(float InScore);
	
private:
 /**
	 @brief 모든 플레이어에게 게임 종료를 알리는 멀티캐스트 함수
 **/
	void ShowGameResult(EGameEndType GameEndType);
public:
	UFUNCTION()
	void RemainTimeUp(int32 TimeAmount);
	
	UFUNCTION()
	void RemainTimeDown(int32 TimeAmount);
public:
 /**
     @brief 탈출, 체포 시에 게임 종료 조건 판단하는 함수
 **/
	void CheckGameEndCondition();
private:
	UPROPERTY()
	TObjectPtr<AACMainGameMode> GameMode;

	float GameScoreGauge = 5000.0f;
	float MafiaWinThreshold = 0.0f;
	float PoliceWinThreshold = 7000.0f;

	
	
protected:
	UPROPERTY()
	uint8 bTimerFlag:1 = false;
	
	int32 GameOverTime;
	
	FTimerHandle GameOverTimeHandle;
	
	UFUNCTION()
	void CheckEndTimer();
};
