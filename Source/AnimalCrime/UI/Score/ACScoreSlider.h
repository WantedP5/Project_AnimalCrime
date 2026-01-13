
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACScoreSlider.generated.h"

UCLASS()
class ANIMALCRIME_API UACScoreSlider : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateScore(float CurrentScore, float MaxScore);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USlider> ScoreSlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ScoreText;
};
