
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACGameResultWinnerBox.generated.h"

UCLASS()
class ANIMALCRIME_API UACGameResultWinnerBox : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetWinnerInfo(class AACPlayerState* WinnerPlayerState);
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> WinnerAvatar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> WinnerNameText;
};
