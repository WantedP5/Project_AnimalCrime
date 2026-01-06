
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACLobbyHeadInfo.generated.h"

UENUM(BlueprintType)
enum class ELobbyPlayerState : uint8
{
	Host,
	Ready,
	UnReady
};

UCLASS()
class ANIMALCRIME_API UACLobbyHeadInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetSteamNameText(const FString& InSteamName);
	UFUNCTION(BlueprintCallable)
	void SetReadyIcon(ELobbyPlayerState NewPlayerState);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> SteamNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> StateIcon;

	//!< BP에서 설정
	UPROPERTY(EditDefaultsOnly, Category = "Lobby|Icon")
	TObjectPtr<class UTexture2D> HostIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Lobby|Icon")
	TObjectPtr<class UTexture2D> ReadyIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Lobby|Icon")
	TObjectPtr<class UTexture2D> UnReadyIcon;
};
