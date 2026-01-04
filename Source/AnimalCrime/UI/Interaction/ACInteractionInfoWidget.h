// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACInteractionInfoWidget.generated.h"

// Forward declarations
class UACInteractionData;

/**
 * @brief 상호작용 가능 액터 위에 표시되는 힌트 위젯
 * @details "[Key] Mission Name" 형식으로 상호작용 정보 표시
 * 예: [E] 신분증 확인
 *      [R] 체포
 */
UCLASS()
class ANIMALCRIME_API UACInteractionInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief 상호작용 데이터로 위젯 내용 업데이트
	 * @param Interactions 유효한 상호작용 배열 (이미 상태별로 필터링됨)
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void UpdateInteractions(const TArray<UACInteractionData*>& Interactions);

	/**
	 * @brief 위젯 표시
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ShowWidget();

	/**
	 * @brief 위젯 숨김
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void HideWidget();

protected:
	/**
	 * @brief 상호작용 힌트 텍스트 블록
	 * @details 여러 줄 지원 (다중 상호작용 표시용)
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> InteractionText;

	/**
	 * @brief 배경/테두리
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBorder> BackgroundBorder;
};
