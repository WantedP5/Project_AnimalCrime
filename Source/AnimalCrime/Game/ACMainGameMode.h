// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ACMainGameMode.generated.h"

USTRUCT()
struct FOutfitCombo
{
	GENERATED_BODY()
	
	// 머리, 상의, 하의 ID
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USkeletalMesh> HairAsset;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USkeletalMesh> TopAsset;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USkeletalMesh> BottomAsset;

	// 생성자
	FOutfitCombo() {}
};
	

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API AACMainGameMode : public AGameMode
{
	GENERATED_BODY()

#pragma region 생성자
public:
	AACMainGameMode();
#pragma endregion
	
#pragma region 엔진 제공 함수
public:
	virtual void BeginPlay() override;
	//virtual AActor* ChoosePlayerStart(AController* Player) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
#pragma endregion
	
public:
#pragma region GameRule
	/** BP_Bush 테스트하던 용도. */
	UFUNCTION(BlueprintCallable)
	void AddTeamScore(int32 Score);
	
	/** BP_Bush 테스트하던 용도. */
	UFUNCTION(BlueprintCallable)
	int32 GetTeamScore() const;
	
	// 여기가 진짜 구현부
	UFUNCTION(BlueprintCallable)
	void UpdateGameScoreFromPolice(EPoliceAction PoliceActionType, float Score);
	
	/** 마피아의 행동에 따른 점수 변동 함수 */
	UFUNCTION(BlueprintCallable)
	void UpdateGameScoreFromMafia(EMafiaAction MafiaActionType, float Score);
	
	UFUNCTION(BlueprintCallable)
	void UpdateGameScoreFromObject(float Score);
	
	UFUNCTION(BlueprintCallable)
	float GetGameScore() const;
#pragma endregion

public:
	void SpawnAllAI();
	FVector GetRandomSpawnLocation() const;
	
	
	void GenerateOutfitPool();
	FOutfitCombo GiveOutfitFromPool();
	
private:
	UPROPERTY(EditAnywhere, meta=(AllowedClasses=Actor))
	TArray<TObjectPtr<class AACCitizen>> AIObjectArray;
	
	UPROPERTY(EditAnywhere, meta=(AllowedClasses=Actor))
	int32 AINums = 5;
	
	UPROPERTY()
	TArray<FOutfitCombo> OutfitPool;
	
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<class USkeletalMesh>> HairList;
	
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<class USkeletalMesh>> TopList;
	
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<class USkeletalMesh>> BottomList;
	
	int32 NextOutfitIndex = 0;
	
#pragma region 게임 종료 판단을 관리하는 맴버 변수
private:
	/** 게임 Score와 종료 시점을 판단하는 매니저 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	TObjectPtr<class UACGameRuleManager> GameRuleManager;
#pragma endregion 

	virtual void PostSeamlessTravel() override;
	
public:
	void HandleVictory();

private:
	FTimerHandle TimerHandle_NextMap;
	void LoadNextMap();
};
