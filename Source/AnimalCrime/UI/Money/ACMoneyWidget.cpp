// Fill out your copyright notice in the Description page of Project Settings.


#include "ACMoneyWidget.h"

#include "Components/TextBlock.h"

void UACMoneyWidget::UpdateMoney(int32 InMoney)
{
	UE_LOG(LogTemp, Log, TEXT("[UACMoneyWidget::UpdateMoney]"));
	if (MoneyText == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("MoneyText nullptr"));
		return ;
	}
	
	//MoneyText->SetText(FText::AsNumber(InMoney));

	MoneyText->SetText(
		FText::Format(
			FText::FromString("$: {0}"),
			FText::AsNumber(InMoney)
		)
	);
}
