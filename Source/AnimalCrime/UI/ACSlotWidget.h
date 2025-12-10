// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ACCustomWidget.h"
#include "ACSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACSlotWidget : public UACCustomWidget
{
	GENERATED_BODY()
	
public:

 /**
     @brief 아이템 데이터 설정
     @param InItemData - 아이템 데이터
 **/
	UFUNCTION(BlueprintCallable)
	void SetItemData(class UACItemData* InItemData);

protected:
	virtual void NativeConstruct() override;

 /**
     @brief 구매 버튼 클릭
 **/
	UFUNCTION()
	void OnPurchaseButtonClicked();

protected:
    // === 위젯 바인딩 (블루프린트 UMG에서 같은 이름으로 생성) ===

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> ItemPreviewImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> ItemNameText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> ItemPriceText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> PurchaseButton;

    // === 런타임 데이터 ===

    // 현재 아이템
    UPROPERTY(BlueprintReadOnly, Category = "Shop")
    TObjectPtr<class UACItemData> ItemData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TObjectPtr<class UMaterialInterface> ItemPreviewMaterial;
};
