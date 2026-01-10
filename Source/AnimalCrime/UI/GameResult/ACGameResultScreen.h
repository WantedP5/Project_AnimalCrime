
#pragma once

#include "CoreMinimal.h"
#include "Game/ACGameEnums.h"
#include "Blueprint/UserWidget.h"
#include "ACGameResultScreen.generated.h"

UCLASS()
class ANIMALCRIME_API UACGameResultScreen : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
public:
	UFUNCTION()
	void SetGameResult(EGameEndType GameEndType);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> GameResultText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FadeInAnim;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> WinnerBoxes;

	//!< BP에서 설정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Winner")
	TSubclassOf<class UACGameResultWinnerBox> WinnerBoxClass;
};
