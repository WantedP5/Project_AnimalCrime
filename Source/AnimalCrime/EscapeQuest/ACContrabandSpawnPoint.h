
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACContrabandSpawnPoint.generated.h"

UCLASS()
class ANIMALCRIME_API AACContrabandSpawnPoint : public AActor
{
	GENERATED_BODY()

public:
    AACContrabandSpawnPoint();

private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<class USceneComponent> SceneRoot;

#if WITH_EDITORONLY_DATA
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UBillboardComponent> Billboard;
#endif
};
