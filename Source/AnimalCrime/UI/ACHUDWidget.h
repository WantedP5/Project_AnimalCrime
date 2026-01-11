// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/ACCustomWidget.h"
#include "ACHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACHUDWidget : public UACCustomWidget
{
	GENERATED_BODY()
	
public:
	void BindGameState();
	void BindPlayerState();
	
	UFUNCTION()
	void HandleScoreChanged(float NewScore);
	
	UFUNCTION()
	void HandleMoneyChanged(int32 NewMoney);
	
	UFUNCTION()
	void HandleAmmoChanged(int32 InAmmo);

	UFUNCTION()
	void HandleGaugeChanged(int32 InSprintGauge);
	
	void ShowSprintUI();
	void HideSprintUI();
	
	// 새 함수 추가 - MoneyComponent 바인딩
	void BindSprintGauge();
	
	// 새 함수 추가 - MoneyComponent 바인딩
	void BindMoneyComponent();

	// ===== 추가: 바운드 아이템 관련 =====

	  /**
	   * @brief PlayerController 바인딩 (바운드 아이템용)
	   */
	void BindBoundItems();

	/**
	 * @brief 무전기 소지 여부 변경 시 호출
	 */
	UFUNCTION()
	void HandleWalkyTalkyChanged(bool bHasWalkyTalky);

	/**
	 * @brief 밀수품 소지 여부 변경 시 호출
	 */
	UFUNCTION()
	void HandleContrabandChanged(bool bHasContraband);
	
	void ZoomInState();
	void ZoomOutState();
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACScoreWidget> WBP_Score;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACQuickSlotWidget> WBP_QuickSlot;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACMoneyWidget> WBP_Money;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACInteractProgressWidget> WBP_InteractProgress;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACAmmoWidget> WBP_Ammo;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACCrossHairWidget> WBP_CrossHair;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACBoundItemWidget> WBP_WalkyTalky;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACBoundItemWidget> WBP_Contraband;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACSprintWidget> WBP_Sprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoundItem")
	TObjectPtr<UTexture2D> WalkyTalkyImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoundItem")
	TObjectPtr<UTexture2D> ContrabandImage;

	

};
