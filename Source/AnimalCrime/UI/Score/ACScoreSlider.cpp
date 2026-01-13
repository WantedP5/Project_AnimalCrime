

#include "UI/Score/ACScoreSlider.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"

void UACScoreSlider::UpdateScore(float CurrentScore, float MaxScore)
{
	CurrentScore = FMath::Clamp(CurrentScore, 0.f, MaxScore);
	const float Percent = FMath::Clamp(CurrentScore / MaxScore, 0.f, 1.f);

	UE_LOG(LogTemp, Log, TEXT("[UACScoreWidget::UpdateScore]"));
	if (ScoreSlider == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ScoreProgressBar nullptr"));
		return;
	}
	ScoreSlider->SetValue(CurrentScore);

	if (ScoreText == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ScoreText nullptr"));
		return;
	}
	ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentScore, MaxScore)));
}
