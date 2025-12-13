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
	
public:
	AACMainGameMode();
	
public:
	virtual void BeginPlay() override;
public:
	UFUNCTION(BlueprintCallable)
	void AddTeamScore(int32 Score);
	
	UFUNCTION(BlueprintCallable)
	int32 GetTeamScore() const;

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
};
