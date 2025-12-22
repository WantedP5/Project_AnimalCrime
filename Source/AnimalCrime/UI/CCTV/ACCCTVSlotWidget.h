// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ACCustomWidget.h"
#include "ACCCTVSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACCCTVSlotWidget : public UACCustomWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	/**
		  @brief CCTV 슬롯 데이터 설정
		  @param Material - CCTV 렌더 텍스처 머티리얼 (M_CCTVArea1~4)
		  @param CCTVName - CCTV 이름 (CCTV1~4)
	  **/
	void SetCCTVData(class UMaterialInterface* Material, const FString& CCTVName);
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> CCTVAreaImg;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CCTVNum;
};
