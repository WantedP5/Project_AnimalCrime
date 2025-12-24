// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Interaction/ACInteractProgressWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "AnimalCrime.h"

void UACInteractProgressWidget::SetProgress(float Progress)
{
	if (ProgressBar == nullptr)
	{
		return;
	}
	ProgressBar->SetPercent(Progress);

	if (ProgressPercentageText == nullptr)
	{
		return;
	}
	int32 Percentage = FMath::RoundToInt(Progress * 100.f);
	ProgressPercentageText->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), Percentage)));
}

void UACInteractProgressWidget::SetTargetName(const FString& Name)
{
	if (TargetNameText == nullptr)
	{
		return;
	}

	TargetNameText->SetText(FText::FromString(FString::Printf(TEXT("%s 상호작용 중..."), *Name)));
}

void UACInteractProgressWidget::ShowWidget()
{
	//UE_LOG(LogSW, Log, TEXT("Shown"));
	SetVisibility(ESlateVisibility::Visible);
}

void UACInteractProgressWidget::HideWidget()
{
	//UE_LOG(LogSW, Log, TEXT("Hid"));
	SetVisibility(ESlateVisibility::Hidden);
}
