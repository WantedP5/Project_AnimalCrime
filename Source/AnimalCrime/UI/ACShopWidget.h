// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACCustomWidget.h"
#include "Components/Border.h"
#include "ACShopWidget.generated.h"

UENUM()
enum class EBorderType
{
	Weapon = 0,
	Clothing,
	End
};

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACShopWidget : public UACCustomWidget
{
	GENERATED_BODY()
	
public:

	UACShopWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	/**
		@brief 모든 아이템 데이터를 로드하여 슬롯 생성
		@param SearchPath - 아이템 검색 경로 (예: "/Game/Item/")
	**/
	UFUNCTION(BlueprintCallable, Category = "Shop")
	void LoadAndCreateSlots(const FString& SearchPath = TEXT("/Game/Project/Item/"));

	/**
		@brief 특정 타입의 아이템만 슬롯 생성
		@param ItemType - 생성할 아이템 타입
	**/
	UFUNCTION(BlueprintCallable, Category = "Shop")
	void CreateSlotsForType(EBorderType Type);

private:

	UFUNCTION()
	void OnWeaponButtonClicked();

	UFUNCTION()
	void OnClothingButtonClicked();

	void ShowBorder(EBorderType Type);

protected:

	// ===== 위젯 바인딩 =====

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> WeaponButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ClothingButton;

	/*UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBorder> WeaponBorder;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBorder> ClothingBorder;*/


	/*UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> WeaponScrollBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> ClothingScrollBox;*/

	/*UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUniformGridPanel> WeaponGridPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUniformGridPanel> ClothingGridPanel;*/

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBorder> ContentBorder;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> ContentScrollBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUniformGridPanel> ContentGridPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> MyMoneyText;

	//UPROPERTY()
	//TArray<TObjectPtr<class UBorder>> Borders;

	// ===== 설정 =====
	
	// 생성할 슬롯 위젯 클래스 (블루프린트에서 설정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TSubclassOf<class UACSlotWidget> SlotWidgetClass;

	// ===== 런타임 데이터 =====

	// 로드된 모든 아이템 데이터
	UPROPERTY(BlueprintReadOnly, Category = "Shop")
	TArray<TObjectPtr<class UACItemData>> AllItemData;

	// 무기 아이템만
	UPROPERTY(BlueprintReadOnly, Category = "Shop")
	TArray<TObjectPtr<class UACItemData>> WeaponItems;

	// 의류 아이템만
	UPROPERTY(BlueprintReadOnly, Category = "Shop")
	TArray<TObjectPtr<class UACItemData>> ClothingItems;

	// 현재 선택된 타입
	UPROPERTY()
	EBorderType CurrentBorderType;
};
