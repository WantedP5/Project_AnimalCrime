
#include "Game/ACPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "AnimalCrime.h"

void AACPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AACPlayerState, PlayerRole);
	DOREPLIFETIME(AACPlayerState, Money);
}


void AACPlayerState::SetMoney(int32 InMoney)
{
	UE_LOG(LogTemp, Error, TEXT("My: %d Input: %d"), Money, InMoney);
	if (Money == InMoney)
	{
		return;
	}

	Money = InMoney;
	OnMoneyChanged.Broadcast(Money);
}

void AACPlayerState::OnRep_Money()
{
	UE_LOG(LogTemp, Error, TEXT("[OnRep_Money]"));
	OnMoneyChanged.Broadcast(Money);
}
