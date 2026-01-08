// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ACCustomWidget.h"
#include "ACPhoneWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACPhoneWidget : public UACCustomWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	// 버튼 클릭 핸들러들
	UFUNCTION()
	void OnCCTVButton1Clicked();

	UFUNCTION()
	void OnCCTVButton2Clicked();

	UFUNCTION()
	void OnCCTVButton3Clicked();

	UFUNCTION()
	void OnCCTVButton4Clicked();

	UFUNCTION()
	void OnHomeButtonClicked();

	// CCTV 데이터 초기화
	void InitializeCCTVSlots();

	// ===== 화면 전환 함수 =====
	void ShowHomeScreen();
	void ShowCCTVScreen(int32 CCTVIndex);

	// CCTVArea를 찾는 함수
	class AACCCTVArea* FindCCTVArea();

protected:
	// CCTV 버튼들
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> CCTVButton1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> CCTVButton2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> CCTVButton3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> CCTVButton4;

	// CCTV 슬롯들
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACCCTVSlotWidget> CCTVSlot1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACCCTVSlotWidget> CCTVSlot2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACCCTVSlotWidget> CCTVSlot3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACCCTVSlotWidget> CCTVSlot4;

	// CCTV 이름들
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CCTVText1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CCTVText2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CCTVText3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CCTVText4;

	// WidgetSwitcher
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> CCTVSwitcher;

	// ===== 메인 화면 전환 =====
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> MainSwitcher;

	// ===== 뒤로가기 버튼 =====
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> HomeButton;

	// CCTV 슬롯 데이터들 (에디터에서 설정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CCTV")
	TArray<struct FCCTVSlotData> CCTVSlotDataArray;

	// 캐싱된 CCTVArea
	UPROPERTY()
	TObjectPtr<class AACCCTVArea> CachedCCTVArea;

	// CCTV를 보고 있는지 여부
	bool bIsViewingCCTV = false;

	// 현재 보고 있는 CCTV 인덱스
	int32 CurrentCCTVIndex = -1;
};
