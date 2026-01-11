// Fill out your copyright notice in the Description page of Project Settings.


#include "ACSprintWidget.h"

#include "AnimalCrime.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"

void UACSprintWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 초기화
	SprintImages.Empty();

	
	const int32 Count = SprintBox->GetChildrenCount();
	for (int32 i = 0; i < Count; i++)
	{
		UImage* Image = Cast<UImage>(SprintBox->GetChildAt(i));
		if (Image == nullptr)
		{
			continue;
		}
		
		// 존재할 경우 추가
		SprintImages.Add(Image);
	}
}

void UACSprintWidget::UpdateSprintGauge(int32 InGauge)
{
	int32 ImageCount = SprintBox->GetChildrenCount();
	UE_LOG(LogHY, Error, TEXT("UpdateSprintGauge %d %d"), InGauge, ImageCount);
	
	
	for (int32 i = ImageCount - 1 ; i >= InGauge ; i--)
	{
		SprintImages[i]->SetVisibility(ESlateVisibility::Hidden);
	}
	
	for (int32 i = InGauge - 1 ; i >= 0 ; i--)
	{
		SprintImages[i]->SetVisibility(ESlateVisibility::Visible);
	}
}

void UACSprintWidget::ShowSprintWidget()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UACSprintWidget::HideSprintWidget()
{
	SetVisibility(ESlateVisibility::Hidden);
}
