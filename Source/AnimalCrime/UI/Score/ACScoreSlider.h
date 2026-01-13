
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Styling/SlateTypes.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HandleImage")
	TObjectPtr<UTexture2D> CryingImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HandleImage")
	TObjectPtr<UTexture2D> SmileImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HandleImage")
	FVector2D HandleImageSize = FVector2D(64.f, 64.f);

	bool bIsHappy = true;
	FSliderStyle SliderStyle;
};
