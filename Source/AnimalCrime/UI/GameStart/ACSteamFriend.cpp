
#include "ACSteamFriend.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "AdvancedSteamFriendsLibrary.h"
#include "AdvancedFriendsLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "AnimalCrime.h"

void UACSteamFriend::NativeConstruct()
{
	Super::NativeConstruct();

	if (InviteButton == nullptr)
	{
		UE_LOG(LogSY, Warning, TEXT("Invite Button not bind"));
		return;
	}

	if (InviteButton->OnClicked.IsAlreadyBound(this, &UACSteamFriend::OnInviteButtonClicked) == false)
	{
		InviteButton->OnClicked.AddDynamic(this, &UACSteamFriend::OnInviteButtonClicked);
	}
}

void UACSteamFriend::UpdateFriend(const FBPFriendInfo& NewFriendDataInfo)
{
	FriendDataInfo = NewFriendDataInfo;

	// Steam 친구 offline 이면 투명하게
	SetRenderOpacity((FriendDataInfo.OnlineState == EBPOnlinePresenceState::Offline) ? 0.25f : 1.0f);

	// Steam 친구 이름 가져오기
	PlayerNameText->SetText(FText::FromString(FriendDataInfo.DisplayName));

	// Steam 친구 아바타 가져오기
	EBlueprintAsyncResultSwitch Result; //가져오는 데 성공했는지를 저장하는 변수
	UTexture2D* AvatarTexture = UAdvancedSteamFriendsLibrary::GetSteamFriendAvatar(
		FriendDataInfo.UniqueNetId,
		Result,
		SteamAvatarSize::SteamAvatar_Medium
	);


	if (Result != EBlueprintAsyncResultSwitch::OnSuccess)
	{
		UE_LOG(LogSY, Warning, TEXT("EBlueprintAsyncResultSwitch is not OnSuccess"));
		return;
	}
	if (AvatarTexture == nullptr)
	{
		UE_LOG(LogSY, Warning, TEXT("AvatarTexture is nullptr"));
		return;
	}
	AvatarImage->SetBrushFromTexture(AvatarTexture);

}

void UACSteamFriend::OnInviteButtonClicked()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);

	if (PC == nullptr)
	{
		return;
	}

	EBlueprintResultSwitch Result; //가져오는 데 성공했는지를 저장하는 변수
	UAdvancedFriendsLibrary::SendSessionInviteToFriend(
		PC,
		FriendDataInfo.UniqueNetId,
		Result
	);

	if (Result == EBlueprintResultSwitch::OnSuccess)
	{
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				3.f,
				FColor::Green,
				TEXT("Session invite sent successfully")
			);
		}
		UE_LOG(LogSY, Log, TEXT("Session invite sent successfully"));
	}
	else
	{
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				3.f,
				FColor::Red,
				TEXT("Failed to send session invite")
			);
		}
		UE_LOG(LogSY, Warning, TEXT("Failed to send session invite"));
	}
}
