#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACBlackMarketComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANIMALCRIME_API UACBlackMarketComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UACBlackMarketComponent();
	void OpenBlackMarket(class AACCharacter* InteractingPlayer);
protected:
	virtual void BeginPlay() override;
};
