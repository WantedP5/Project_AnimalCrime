#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/ACGameEnums.h"
#include "ACArea.h"
#include "ACBombInstallArea.generated.h"

UCLASS()
class ANIMALCRIME_API AACBombInstallArea : public AACArea
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBombTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBombDestroyComplete(AACEscapeMissionBomb* Bomb);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spot")
	ESpot BombSpot = ESpot::Bank;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb")
	TObjectPtr<class AACDestructibleBuilding> DestructibleBuilding;
};
