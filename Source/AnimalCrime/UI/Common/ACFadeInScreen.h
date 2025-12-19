// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACFadeInScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFadeFinishedDelegate);
UCLASS()
class ANIMALCRIME_API UACFadeInScreen : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void HandleFadeInFinished();

public:
    UPROPERTY(BlueprintAssignable)
    FFadeFinishedDelegate OnFadeInFinished;

protected:
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* FadeInAnim;
};
