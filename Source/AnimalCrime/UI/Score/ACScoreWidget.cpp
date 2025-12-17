// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Score/ACScoreWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UACScoreWidget::UpdateScore(float CurrentScore, float MaxScore)
{
	const float Percent = FMath::Clamp(CurrentScore / MaxScore, 0.f, 1.f);

	UE_LOG(LogTemp, Log, TEXT("[UACScoreWidget::UpdateScore]"));
	if (ScoreProgressBar == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ScoreProgressBar nullptr"));
		return ;
	}
	ScoreProgressBar->SetPercent(Percent);

	if (ScoreText == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ScoreText nullptr"));
		return ;
	}
	ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%f / %f"), CurrentScore, MaxScore)));
}
