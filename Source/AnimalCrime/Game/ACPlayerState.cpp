
#include "ACPlayerState.h"
#include "ACMainGameState.h"
#include "Net/UnrealNetwork.h"
#include "AnimalCrime.h"

void AACPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AACPlayerState, PlayerRole);
}

void AACPlayerState::EnterSpectatorState()
{
	if (IsSpectator() == true)
	{
		return;
	}

	//관전 상태로 전환
	SetIsSpectator(true); 

	if (HasAuthority() == false)
	{
		return;
	}

	AACMainGameState* GS = GetWorld()->GetGameState<AACMainGameState>();
	if (GS == nullptr)
	{
		return;
	}

	AController* PC = GetOwner<AController>();
	if (PC == nullptr)
	{
		return;
	}

	GS->RemoveSpectatablePawn(PC->GetPawn());
}
