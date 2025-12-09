#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACBombInstallArea.generated.h"

UCLASS()
class ANIMALCRIME_API ACBombInstallArea : public AActor
{
	GENERATED_BODY()

public:
	ACBombInstallArea();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBombTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UBoxComponent> TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UDecalComponent> VisualBox;
};
