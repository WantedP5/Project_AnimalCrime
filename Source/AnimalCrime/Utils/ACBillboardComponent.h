
#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ACBillboardComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANIMALCRIME_API UACBillboardComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UACBillboardComponent();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
