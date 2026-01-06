
#include "UI/GameStart/ACLobbyHeadInfo.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "AnimalCrime.h"

void UACLobbyHeadInfo::SetSteamNameText(const FString& InSteamName)
{
	if (SteamNameText == nullptr)
	{
		return;
	}
	SteamNameText->SetText(FText::FromString(InSteamName));
}

void UACLobbyHeadInfo::SetReadyIcon(ELobbyPlayerState NewPlayerState)
{

	UTexture2D* TargetTexture = nullptr;

	switch (NewPlayerState)
	{
	case ELobbyPlayerState::Host:
		TargetTexture = HostIcon;
		break;

	case ELobbyPlayerState::Ready:
		TargetTexture = ReadyIcon;
		break;

	case ELobbyPlayerState::UnReady:
		TargetTexture = UnReadyIcon;
		break;
	}

	if (TargetTexture == nullptr)
	{
		UE_LOG(LogSY, Warning, TEXT("UACLobbyHeadInfo::SetReadyIcon TargetTexture is nullptr"));
		return;
	}

	StateIcon->SetBrushFromTexture(TargetTexture, true);
	StateIcon->SetVisibility(ESlateVisibility::Visible);
}
