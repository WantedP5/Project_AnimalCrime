
#include "Character/ACLobbyCharacter.h"
#include "Game/ACLobbyPlayerController.h"
#include "EnhancedInputComponent.h"

#include "AnimalCrime.h"

AACLobbyCharacter::AACLobbyCharacter()
{
	HeadMesh->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Hat_057.SK_Hat_057")));
}

void AACLobbyCharacter::SetSteamFriendsList(const FInputActionValue& Value)
{
	AACLobbyPlayerController* PC = Cast<AACLobbyPlayerController>(GetController());
	if (PC == nullptr)
	{
		return;
	}

	//설정창이 꺼져있으면 스팀친구창 오픈, 스팀 친구창이 켜져있으면 끄기, 다른 설정창이면 아무것도 안함.
	if (SettingMode == ESettingMode::None)
	{
		PC->SteamFriendListToggle(true);
		ChangeInputMode(EInputMode::Settings);
		SettingMode = ESettingMode::SteamFriendList;
	}
	else if (SettingMode == ESettingMode::SteamFriendList)
	{
		PC->SteamFriendListToggle(false);
		ChangeInputMode(EInputMode::Sholder);
		SettingMode = ESettingMode::None;
	}
}

void AACLobbyCharacter::SettingsClose(const FInputActionValue& Value)
{
	switch (SettingMode)
	{
	case ESettingMode::None:
		break;
	case ESettingMode::Default:
		break;
	case ESettingMode::SteamFriendList:
		SetSteamFriendsList(Value);
		break;
	default:
		break;
	}
}

//void AACLobbyCharacter::GameReady(const FInputActionValue& Value)
//{
//	AACLobbyPlayerController* PC = Cast<AACLobbyPlayerController>(GetController());
//	if (PC == nullptr)
//	{
//		return;
//	}
//
//	// 호스트인지 확인
//	if (PC->IsLocalController() && HasAuthority())
//	{
//		AC_LOG(LogSY, Log, TEXT("호스트 맞음"));
//		//맵 이동
//		PC->ServerStartGame();
//	}
//	else
//	{
//		AC_LOG(LogSY, Log, TEXT("호스트 아님"));
//		PC->ServerReadyToggle();
//	}
//}
