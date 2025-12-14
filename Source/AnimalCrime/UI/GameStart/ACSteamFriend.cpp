
#include "ACSteamFriend.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
//#include "AdvancedSteamFriendsLibrary.h"

void UACSteamFriend::UpdateFriend(const FBPFriendInfo& NewFriendDataInfo)
{
	FriendDataInfo = NewFriendDataInfo;

	SetRenderOpacity((FriendDataInfo.OnlineState == EBPOnlinePresenceState::Offline) ? 0.25f: 1.0f);

	PlayerNameText->SetText(FText::FromString(FriendDataInfo.DisplayName));

	// Steam 친구 아바타 가져오기
	//EBlueprintAsyncResultSwitch Result; //가져오는 데 성공했는지를 저장하는 변수
	//UTexture2D* AvatarTexture = UAdvancedSteamFriendsLibrary::GetSteamFriendAvatar(
	//	FriendDataInfo.UniqueNetId,
	//	Result,
	//	SteamAvatarSize::SteamAvatar_Medium
	//);

	//if (Result != EBlueprintAsyncResultSwitch::OnSuccess)
	//{
	//	return;
	//}

	//if (AvatarTexture == nullptr)
	//{
	//	return;
	//}

	//AvatarImage->SetBrushFromTexture(AvatarTexture);
	
}
