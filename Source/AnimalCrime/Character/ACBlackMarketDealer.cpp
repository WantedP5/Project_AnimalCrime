
#include "ACBlackMarketDealer.h"
#include "ACMafiaCharacter.h"
#include "Components/BoxComponent.h"
#include "EscapeQuest/ACBlackMarketComponent.h"
#include "Component/ACInteractableComponent.h"
#include "AnimalCrime.h"

AACBlackMarketDealer::AACBlackMarketDealer()
{
	HeadMesh->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Hairstyle_male_010.SK_Hairstyle_male_010")));
	TopMesh->SetSkeletalMesh(nullptr);
	BottomMesh->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Costume_6_001.SK_Costume_6_001")));
	BlackMarketComponent = CreateDefaultSubobject<UACBlackMarketComponent>(TEXT("BlackMarketComponent"));

}

EACCharacterType AACBlackMarketDealer::GetCharacterType()
{
	return EACCharacterType::BlackMarketDealer;
}

bool AACBlackMarketDealer::CanInteract(AACCharacter* ACPlayer)
{
	if (ACPlayer == nullptr)
	{
		return false;
	}
	if (ACPlayer->GetCharacterType() != EACCharacterType::Mafia)
	{
		AC_LOG(LogSW, Log, TEXT("Sorry Only For MAFIA!!!!!"));
		return false;
	}

	AC_LOG(LogSW, Log, TEXT("Bomb - Mafia Contacted!!"));
	return true;
}

void AACBlackMarketDealer::OnInteract(AACCharacter* ACPlayer)
{
	//ShowInteractDebug(ACPlayer, GetName());

	if (BlackMarketComponent)
	{
		BlackMarketComponent->OpenBlackMarket(ACPlayer);
	}
}

