
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
protected:
	virtual bool CanInteract(class AACCharacter* ACPlayer) override;		// 누가 상호작용 가능한지(캐릭터 타입 체크) |
	virtual void OnInteract(class AACCharacter* ACPlayer) override;		// 실제 상호작용 로직(서버에서 실행) |
	virtual FString GetInteractableName() const override;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackMarket")
	TObjectPtr<class UACBlackMarketComponent> BlackMarketComponent;

};
