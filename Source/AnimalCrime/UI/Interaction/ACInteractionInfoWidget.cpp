// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Interaction/ACInteractionInfoWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Interaction/ACInteractionData.h"
#include "AnimalCrime.h"

void UACInteractionInfoWidget::UpdateInteractions(const TArray<UACInteractionData*>& Interactions)
{
	if (InteractionText == nullptr)
	{
		return;
	}

	if (Interactions.Num() == 0)
	{
		InteractionText->SetText(FText::GetEmpty());
		return;
	}

	// "[E] 신분증 확인" 형식으로 텍스트 생성
	FString HintText;
	for (int32 i = 0; i < Interactions.Num(); ++i)
	{
		UACInteractionData* Data = Interactions[i];
		if (Data == nullptr)
		{
			continue;
		}

		// "[E] 신분증 확인" 형식
		FString Line = FString::Printf(TEXT("[%s] %s"),
			*Data->AssignedKey,
			*Data->InteractionName.ToString());

		HintText += Line;

		// 마지막이 아니면 줄바꿈
		if (i < Interactions.Num() - 1)
		{
			HintText += TEXT("\n");
		}
	}

	InteractionText->SetText(FText::FromString(HintText));
}

void UACInteractionInfoWidget::ShowWidget()
{
	//UE_LOG(LogSW, Log, TEXT("SHOW UI"));
	SetVisibility(ESlateVisibility::Visible);
}

void UACInteractionInfoWidget::HideWidget()
{
	//UE_LOG(LogSW, Log, TEXT("HIDE UI"));
	SetVisibility(ESlateVisibility::Hidden);
}
