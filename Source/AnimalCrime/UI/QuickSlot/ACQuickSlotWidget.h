// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ACCustomWidget.h"
#include "ACQuickSlotWidget.generated.h"

/**

    @class   UACQuickSlotWidget
    @brief   무기 아이템이 들어갈 퀵 슬롯 클래스
    @details HUD에 띄울 예정

**/
UCLASS()
class ANIMALCRIME_API UACQuickSlotWidget : public UACCustomWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
    /**
        @brief 퀵슬롯에 아이템 추가하기
        @param ItemData    - 추가할 아이템 정보
        @param bIsMainSlot - true면 메인슬롯, false면 서브슬롯
    **/
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void AddItem(class UACItemData* ItemData, bool bIsMainSlot = true);

    /**
          @brief  퀵슬롯에 아이템을 자동으로 추가 (빈 슬롯 찾아서)
          @param  ItemData - 추가할 아이템 정보
          @retval true: 추가 성공, false: 슬롯이 꽉 참
      **/
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    bool TryAddItem(class UACItemData* ItemData);

    /**
         @brief  슬롯 아이템 장착/해제 토글 (통합 버전)
         @param  SlotIndex - 슬롯 번호 (1: 메인슬롯, 2: 서브슬롯)
     **/
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void ToggleSlotEquip(int32 SlotIndex);

    /**
         @brief  메인 슬롯이 비어있는지 확인
         @retval true: 비어있음, false: 차있음
     **/
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    bool IsMainSlotEmpty() const { return MainSlotItem == nullptr; }

    /**
        @brief  서브 슬롯이 비어있는지 확인
        @retval true: 비어있음, false: 차있음
    **/
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    bool IsSubSlotEmpty() const { return SubSlotItem == nullptr; }

    /**
       @brief  퀵슬롯이 꽉 찼는지 확인
       @retval true: 꽉 참, false: 빈 슬롯 있음
   **/
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    bool IsFull() const { return MainSlotItem != nullptr && SubSlotItem != nullptr; }

    /**
        @brief  퀵슬롯 아이템 가져오기
        @retval  - 슬롯별 아이템 정보
    **/
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    class UACItemData* GetMainSlotItem() const { return MainSlotItem; }

    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    class UACItemData* GetSubSlotItem() const { return SubSlotItem; }

protected:
    // === 위젯 바인딩 (블루프린트 UMG에서 같은 이름으로 생성) ===

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UBorder> MainSlot;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UBorder> SubSlot;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UBorder> MainSlotHighlight;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UBorder> SubSlotHighlight;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> MainItemImg;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> SubItemImg;

    // 슬롯에 저장된 아이템
    UPROPERTY(BlueprintReadOnly, Category = "QuickSlot")
    TObjectPtr<class UACItemData> MainSlotItem;

    UPROPERTY(BlueprintReadOnly, Category = "QuickSlot")
    TObjectPtr<class UACItemData> SubSlotItem;
};
