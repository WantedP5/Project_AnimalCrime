
#include "UI/GameStart/ACLobbyScreen.h"
#include "Components/TextBlock.h"
#include "Game/ACLobbyGameState.h"
#include "AnimalCrime.h"

void UACLobbyScreen::NativeConstruct()
{
	Super::NativeConstruct();
	AACLobbyGameState* GS = GetWorld()->GetGameState<AACLobbyGameState>();
	if (GS == nullptr)
	{	
		return;
	}
	
	//ReadyPlayer, Player 수가 변경되면 호출되도록 게임 스테이트 구독
	GS->OnPlayerCountChanged.AddDynamic(this, &UACLobbyScreen::UpdatePlayerReadyText);

	// 처음에는 ReadyPlayer, Player 수 직접 갱신
	UpdatePlayerReadyText(GS->GetReadyPlayerCount(), GS->GetAllPlayerCount());
}

void UACLobbyScreen::UpdatePlayerReadyText(int32 ReadyPlayerNum, int32 AllPlayerNum)
{
	if (ReadyPlayerText == nullptr)
	{
		UE_LOG(LogSY, Warning, TEXT("ReadyPlayerText is nullptr"));
		return;
	}
	UE_LOG(LogSY, Log, TEXT("Update UI Screen"));
	ReadyPlayerText->SetText( FText::Format(FText::FromString(TEXT("{0}/{1}")), ReadyPlayerNum, AllPlayerNum));
}
