// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindGameState();
	
	UFUNCTION()
	void HandleScoreChanged(float NewScore);
	
	UFUNCTION()
	void HandleMoneyChanged(int32 NewMoney);
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACScoreWidget> WBP_Score;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACQuickSlotWidget> WBP_QuickSlot;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACMoneyWidget> WBP_Money;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACInteractProgressWidget> WBP_InteractProgress;
};
