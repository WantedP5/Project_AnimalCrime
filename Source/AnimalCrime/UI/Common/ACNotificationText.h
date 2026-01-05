// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACNotificationText.generated.h"

UCLASS()
class ANIMALCRIME_API UACNotificationText : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void HandleNotificationFinished();
public:
	UFUNCTION(BlueprintCallable)
	void SetNotificationText(const FText& InText);
protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> NotificationText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FlyAnim;
};
