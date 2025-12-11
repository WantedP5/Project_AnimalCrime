
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

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnInteractTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	void OnInteracted(class AACCharacter* InteractingPlayer);

protected:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	//TObjectPtr<class UBoxComponent> InteractBoxComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BlackMarket")
	TObjectPtr<class UACBlackMarketComponent> BlackMarketComponent;

};
