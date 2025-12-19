
#pragma once

#include "CoreMinimal.h"
#include "Character/ACCharacter.h"
#include "ACBlackMarketDealer.generated.h"

UCLASS()
class ANIMALCRIME_API AACBlackMarketDealer : public AACCharacter
{
	GENERATED_BODY()

public:
	AACBlackMarketDealer();

	virtual EACCharacterType GetCharacterType() override;

	//!< 상호작용 인터페이스
protected:
	virtual bool CanInteract(class AACCharacter* ACPlayer) override;
	virtual void OnInteract(class AACCharacter* ACPlayer) override;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackMarket")
	TObjectPtr<class UACBlackMarketComponent> BlackMarketComponent;

};
