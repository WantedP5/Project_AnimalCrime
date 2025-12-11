// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACCustomWidget.h"
#include "ACShopWidget.generated.h"

UENUM()
enum class EShopCategory
{
	Weapon = 0,
	Head,
	FaceAcc,
	Top,
	Bottom,
	Shoes,
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
		@brief 특정 카테고리의 아이템만 슬롯 생성
		@param Category - 생성할 카테고리
	**/
	UFUNCTION(BlueprintCallable, Category = "Shop")
	void CreateSlotsForCategory(EShopCategory Category);

private:

	UFUNCTION()
	void OnWeaponButtonClicked();

	UFUNCTION()
	void OnHeadButtonClicked();

	UFUNCTION()
	void OnFaceAccButtonClicked();

	UFUNCTION()
	void OnTopButtonClicked();

	UFUNCTION()
	void OnBottomButtonClicked();

	UFUNCTION()
	void OnShoesButtonClicked();

	void ShowCategory(EShopCategory Type);

protected:

	// ===== 위젯 바인딩 =====

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> WeaponButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> HeadButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> FaceAccButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> TopButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BottomButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ShoesButton;

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

	// 의류 슬롯별로 분류
	UPROPERTY(BlueprintReadOnly, Category = "Shop")
	TArray<TObjectPtr<class UACItemData>> HeadItems;

	UPROPERTY(BlueprintReadOnly, Category = "Shop")
	TArray<TObjectPtr<class UACItemData>> FaceAccItems;

	UPROPERTY(BlueprintReadOnly, Category = "Shop")
	TArray<TObjectPtr<class UACItemData>> TopItems;

	UPROPERTY(BlueprintReadOnly, Category = "Shop")
	TArray<TObjectPtr<class UACItemData>> BottomItems;

	UPROPERTY(BlueprintReadOnly, Category = "Shop")
	TArray<TObjectPtr<class UACItemData>> ShoesItems;

	// 현재 선택된 카테고리
	UPROPERTY()
	EShopCategory CurrentCategory;
};
