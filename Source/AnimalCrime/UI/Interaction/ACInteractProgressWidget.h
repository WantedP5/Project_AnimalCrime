// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACInteractProgressWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACInteractProgressWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    /**
        @brief 프로그래스 바 업데이트와 퍼센트 텍스트 업데이트
        @param Progress - 진행도 0.0~1.0 사이로 입력받음
    **/
    UFUNCTION(BlueprintCallable)
    void SetProgress(float Progress);

    /**
        @brief 프로그래스 바 밑에 상호작용 물체 이름 텍스트 설정.
        @param Name - 상호작용 물체 이름
    **/
    //todo:  추후에 임무 이름으로 변경 예정.
    UFUNCTION(BlueprintCallable)
    void SetInteractionText(const FString& InteractionName);

    /**
        @brief 위젯 가시화
    **/
    //todo: 부모에 두는건 어떨까
    UFUNCTION(BlueprintCallable)
    void ShowWidget();

    /**
        @brief 위젯 숨김
    **/
    UFUNCTION(BlueprintCallable)
    void HideWidget();

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UProgressBar> ProgressBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> ProgressPercentageText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> InteractionNameText;
	
};
