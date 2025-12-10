#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACArea.h"
#include "ACBombInstallArea.generated.h"

UCLASS()
class ANIMALCRIME_API ACBombInstallArea : public AACArea
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBombTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
