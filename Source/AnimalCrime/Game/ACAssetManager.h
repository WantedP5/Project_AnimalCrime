// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ACAssetManager.generated.h"

/**
 * 커스텀 AssetManager
 * Primary Asset을 자동으로 로드하고 캐싱합니다.
 */
UCLASS()
class ANIMALCRIME_API UACAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	/**
	 * @brief 싱글톤 인스턴스 가져오기
	 */
	static UACAssetManager& Get();

	/**
	 * @brief 엔진 초기화 시 호출 - ItemData를 미리 로드
	 */
	virtual void StartInitialLoading() override;

	/**
	 * @brief 모든 ItemData Primary Asset 가져오기
	 * @return 로드된 모든 ItemData 배열
	 */
	UFUNCTION(BlueprintCallable, Category = "Asset Manager")
	TArray<class UACItemData*> GetAllItemData();

private:
	// 캐시된 ItemData 목록 (첫 호출 시 한 번만 로드)
	UPROPERTY()
	TArray<TObjectPtr<class UACItemData>> CachedItemData;

	// 로드 완료 플래그
	bool bItemDataLoaded = false;

	// 실제 로드 함수
	void LoadItemDataInternal();
};
