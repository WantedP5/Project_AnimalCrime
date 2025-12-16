
#pragma once

#include "CoreMinimal.h"
#include "Character/ACCharacter.h"
#include "ACTestBlackMarketDealer.generated.h"

UCLASS()
class ANIMALCRIME_API AACTestBlackMarketDealer : public AACCharacter
{
	GENERATED_BODY()

public:
	AACTestBlackMarketDealer();

	virtual EACCharacterType GetCharacterType() override;

	//!< 상호작용 인터페이스
protected:
	virtual bool CanInteract(class AACCharacter* ACPlayer) override;
	virtual void OnInteract(class AACCharacter* ACPlayer) override;
	virtual FString GetInteractableName() const override;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackMarket")
	TObjectPtr<class UACBlackMarketComponent> BlackMarketComponent;

};
