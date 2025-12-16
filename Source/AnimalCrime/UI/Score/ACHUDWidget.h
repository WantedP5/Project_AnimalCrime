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
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACScoreWidget> WBP_Score;
};
