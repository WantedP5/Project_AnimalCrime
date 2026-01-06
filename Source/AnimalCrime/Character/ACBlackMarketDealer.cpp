
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

void AACBlackMarketDealer::OnInteract(AACCharacter* ACPlayer, EInteractionKey InKey)
{
	if (BlackMarketComponent)
	{
		BlackMarketComponent->OpenBlackMarket(ACPlayer);
	}
}

EACInteractorType AACBlackMarketDealer::GetInteractorType() const
{
	return EACInteractorType::BlackMarketDealer;
}

