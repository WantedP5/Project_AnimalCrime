// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ACCustomWidget.h"
#include "ACCCTVWidget.generated.h"

 /**
      @struct FCCTVSlotData
      @brief  CCTV 슬롯 하나의 데이터 (머티리얼 + 텍스트)
  */
USTRUCT(BlueprintType)
struct FCCTVSlotData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<class UMaterialInterface> Material = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CCTVName = TEXT("CCTV");
};

/**
     @class   UACCCTVWidget
     @brief   CCTV 화면을 표시하는 위젯
     @details 4개의 CCTV 슬롯을 관리하고 각각에 렌더 텍스처를 표시
 */
UCLASS()
class ANIMALCRIME_API UACCCTVWidget : public UACCustomWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    /**
        @brief CCTV 슬롯들을 초기화 (머티리얼과 텍스트 설정)
    **/
    void InitializeCCTVSlots();

protected:
    // 4개의 CCTV 슬롯
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UACCCTVSlotWidget> CCTVSlot1;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UACCCTVSlotWidget> CCTVSlot2;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UACCCTVSlotWidget> CCTVSlot3;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UACCCTVSlotWidget> CCTVSlot4;

    // CCTV 슬롯 데이터들
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CCTV")
    TArray<FCCTVSlotData> CCTVSlotDataArray;
};
