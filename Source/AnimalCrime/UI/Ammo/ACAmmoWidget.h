// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ACCustomWidget.h"
#include "ACAmmoWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACAmmoWidget : public UACCustomWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateAmmo(int32 InAmmo);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> AmmoText;
};
