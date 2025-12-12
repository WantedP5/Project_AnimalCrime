
#include "ACTestBlackMarketDealer.h"
#include "ACTestMafiaCharacter.h"
#include "Components/BoxComponent.h"
#include "EscapeQuest/ACBlackMarketComponent.h"
#include "Component/ACInteractableComponent.h"
#include "AnimalCrime.h"

AACTestBlackMarketDealer::AACTestBlackMarketDealer()
{
	HeadMesh->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Hairstyle_male_010.SK_Hairstyle_male_010")));
	TopMesh->SetSkeletalMesh(nullptr);
	BottomMesh->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Costume_6_001.SK_Costume_6_001")));
	BlackMarketComponent = CreateDefaultSubobject<UACBlackMarketComponent>(TEXT("BlackMarketComponent"));

}

EACCharacterType AACTestBlackMarketDealer::GetCharacterType()
{
	return EACCharacterType::BlackMarketDealer;
}

bool AACTestBlackMarketDealer::CanInteract(AACCharacter* ACPlayer)
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

void AACTestBlackMarketDealer::OnInteract(AACCharacter* ACPlayer)
{
	ShowInteractDebug(ACPlayer);

	if (BlackMarketComponent)
	{
		BlackMarketComponent->OpenBlackMarket(ACPlayer);
	}
}

FString AACTestBlackMarketDealer::GetInteractableName() const
{
	return TEXT("BlackMarketDealer");
}