// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ACCustomWidget.h"
#include "ACSprintWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACSprintWidget : public UACCustomWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;


protected:
	// === 위젯 바인딩 (블루프린트 UMG에서 같은 이름으로 생성) ===

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> SprintBox;

	UPROPERTY()
	TArray<TObjectPtr<class UImage>> SprintImages;

public:	
	void UpdateSprintGauge(int32 InGauge);
	
	void ShowSprintWidget();
	void HideSprintWidget();
	
};
