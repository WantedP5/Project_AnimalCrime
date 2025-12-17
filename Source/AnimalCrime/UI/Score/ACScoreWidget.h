// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateScore(float CurrentScore, float MaxScore);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ScoreProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ScoreText;
};
