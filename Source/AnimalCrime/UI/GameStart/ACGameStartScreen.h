// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACGameStartScreen.generated.h"

UCLASS()
class ANIMALCRIME_API UACGameStartScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
private:
	UFUNCTION()
	void OnCreateRoomClicked();
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> CreateRoomButton;
};
