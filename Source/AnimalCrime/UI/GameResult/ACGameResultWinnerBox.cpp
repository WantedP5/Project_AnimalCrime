

#include "UI/GameResult/ACGameResultWinnerBox.h"
#include "Game/ACPlayerState.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "AdvancedSteamFriendsLibrary.h"
#include "AdvancedFriendsLibrary.h"

void UACGameResultWinnerBox::SetWinnerInfo(AACPlayerState* WinnerPlayerState)
{
	if (WinnerPlayerState == nullptr)
	{
		return;
	}
	if (WinnerNameText == nullptr)
	{
		return;
	}
	WinnerNameText->SetText(FText::FromString(WinnerPlayerState->GetPlayerName()));

	// Steam 아바타 가져오기
	FBPUniqueNetId UniqueNetId;
	UniqueNetId.SetUniqueNetId(WinnerPlayerState->GetUniqueId().GetUniqueNetId());

	EBlueprintAsyncResultSwitch Result;
	UTexture2D* AvatarTexture = UAdvancedSteamFriendsLibrary::GetSteamFriendAvatar(
		UniqueNetId,
		Result,
		SteamAvatarSize::SteamAvatar_Large
	);

	if (Result != EBlueprintAsyncResultSwitch::OnSuccess || AvatarTexture == nullptr)
	{
		return;
	}

	if (WinnerAvatar == nullptr)
	{
		return;
	}
	WinnerAvatar->SetBrushFromTexture(AvatarTexture);
}
