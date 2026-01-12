
#include "ACGameStartScreen.h"
#include "Components/Button.h"
#include "Game/ACAdvancedFriendsGameInstance.h"

#include "Kismet/KismetSystemLibrary.h"
#include "AnimalCrime.h"
void UACGameStartScreen::NativeConstruct()
{
	if (CreateRoomButton != nullptr)
	{
		CreateRoomButton->OnClicked.AddDynamic(this, &UACGameStartScreen::OnCreateRoomClicked);
	}
}

void UACGameStartScreen::OnCreateRoomClicked()
{
	UE_LOG(LogSY, Log, TEXT("Clicked"));
	UACAdvancedFriendsGameInstance* GI = Cast< UACAdvancedFriendsGameInstance>(GetGameInstance());
	if (GI == nullptr)
	{
		UE_LOG(LogSY, Log, TEXT("ACAdvancedFriendsGameInstance cast fail"));
		return;
	}

	GI->CreateSession();
}
