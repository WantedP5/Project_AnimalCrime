// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Ammo/ACAmmoWidget.h"

#include "Components/TextBlock.h"

void UACAmmoWidget::UpdateAmmo(int32 InAmmo)
{
	UE_LOG(LogTemp, Log, TEXT("[UACMoneyWidget::UpdateMoney]"));
	if (AmmoText == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("MoneyText nullptr"));
		return ;
	}
	
	AmmoText->SetText(FText::AsNumber(InAmmo));
}
