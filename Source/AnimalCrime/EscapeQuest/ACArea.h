
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACArea.generated.h"

UCLASS()
class ANIMALCRIME_API AACArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AACArea();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UBoxComponent> TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UNiagaraComponent> NiagaraEffect;
};
