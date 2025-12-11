
#pragma once

#include "CoreMinimal.h"
#include "EscapeQuest/ACArea.h"
#include "ACEscapeArea.generated.h"

UCLASS()
class ANIMALCRIME_API AACEscapeArea : public AACArea
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnEscapeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
