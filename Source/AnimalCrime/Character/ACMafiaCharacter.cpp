
#include "Character/ACMafiaCharacter.h"
#include "Item/ACEscapeMissionBomb.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "ACBlackMarketDealer.h"
#include "EscapeQuest/ACBombInstallArea.h"
#include "EscapeQuest/ACEscapeArea.h"
#include "Kismet/GameplayStatics.h"

#include "Game/ACMainGameState.h"
#include "AnimalCrime.h"

void AACMafiaCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AACMafiaCharacter, HandBomb);
}

void AACMafiaCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 서버만 GameState에 등록
	if (HasAuthority() == false) 
	{
		return;
	}
	AACMainGameState* GS = GetWorld()->GetGameState<AACMainGameState>();
	if (GS == nullptr)
	{
		return;
	}
	GS->MafiaPlayers.Add(this);
	AC_LOG(LogSY, Warning, TEXT("Mafia:: %d"), GS->MafiaPlayers.Num());
}

bool AACMafiaCharacter::CanInteract(AACCharacter* ACPlayer)
{
	return true;
}

void AACMafiaCharacter::OnInteract(AACCharacter* ACPlayer)
{
	ShowInteractDebug(ACPlayer);
}

FString AACMafiaCharacter::GetInteractableName() const
{
	return TEXT("Mafia");
}

EACCharacterType AACMafiaCharacter::GetCharacterType()
{
	return EACCharacterType::Mafia;
}

void AACMafiaCharacter::ItemDrop(const FInputActionValue& Value)
{
	AC_LOG(LogSY, Log, TEXT("ItemDrop Key!!"));
	ServerItemDrop(); //서버에 알림.
}

void AACMafiaCharacter::ServerItemDrop_Implementation()
{
	if (HandBomb != nullptr)
	{
		//참조 해제
		HandBomb->AttachedCharacter = nullptr;

		//캐릭터에서 폭탄 분리
		HandBomb->DetachFromCharacter();

		// Impulse 적용은 서버가 직접 해야 하므로 여기서 처리
		//UStaticMeshComponent* MeshComp = HandBomb->GetBombMeshComp();
		//if (MeshComp)
		//{
		//    FVector ThrowDir = GetActorForwardVector() + FVector(0, 0, 0.3f);
		//    MeshComp->AddImpulse(ThrowDir * 300.f, NAME_None, true);
		//}

		HandBomb = nullptr;

		//폭탄 설치 가능 구역 숨기기
		
		ClientSetBombAreaVisible(false);
		AC_LOG(LogSY, Log, TEXT("Bomb dropped"));
	}
	else
	{
		AC_LOG(LogSY, Warning, TEXT("No bomb to drop"));
	}
}

void AACMafiaCharacter::ClientSetBombAreaVisible_Implementation(bool bVisible)
{
	AACMainGameState* GS = GetWorld()->GetGameState<AACMainGameState>();
	if (GS == nullptr)
	{
		return;
	}
	AC_LOG(LogSY, Log, TEXT("%d"), GS->BombAreas.Num());
	for (AACBombInstallArea* Area : GS->BombAreas) 
	{
		if (Area==nullptr) 
		{
			continue;
		}

		Area->SetActorHiddenInGame(!bVisible);

		AC_LOG(LogSY, Log, TEXT("ClientSetBombAreaVisible"));
	}
}

void AACMafiaCharacter::ClientSetEscapeAreaVisible_Implementation(bool bVisible)
{
	AACMainGameState* GS = GetWorld()->GetGameState<AACMainGameState>();
	if (GS == nullptr)
	{
		return;
	}
	AC_LOG(LogSY, Log, TEXT("%d"), GS->EscapeAreas.Num());
	for (AACEscapeArea* Area : GS->EscapeAreas)
	{
		if (Area==nullptr)
		{
			continue;
		}
		Area->SetActorHiddenInGame(!bVisible);

		Area->SetActorEnableCollision(true);
		AC_LOG(LogSY, Log, TEXT("ClientSetBombAreaVisible"));
	}
}

void AACMafiaCharacter::OnRep_HandBomb()
{
	if (HandBomb)
	{
		AC_LOG(LogSY, Log, TEXT("OnRep_HandBomb: Now holding bomb"));
	}
	else
	{
		AC_LOG(LogSY, Log, TEXT("OnRep_HandBomb: No longer holding bomb"));
	}
}
