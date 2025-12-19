// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACMoneyWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACMoneyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateMoney(int32 InMoney);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> MoneyText;
};
