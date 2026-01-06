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

	// 새 함수 추가 - MoneyComponent 바인딩
	void BindMoneyComponent();
	
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
};
