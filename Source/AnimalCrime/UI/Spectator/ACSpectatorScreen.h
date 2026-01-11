
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACSpectatorScreen.generated.h"


UCLASS()
class ANIMALCRIME_API UACSpectatorScreen : public UUserWidget
{
	GENERATED_BODY()
public:
	void BindGameState();
	UFUNCTION()
	void HandleScoreChanged(float NewScore);

	UFUNCTION()
	void HandleEscapeCountChanged(int32 NewCount);
	
	void SetFriend(class AACPlayerState* PS);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UACScoreWidget> WBP_Score;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> FriendImage;

	UPROPERTY(meta = (BindWidget)) 
	TObjectPtr<class UTextBlock> FriendNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> EscapeNumText;
};
