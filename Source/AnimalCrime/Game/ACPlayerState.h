
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Interface/ACInteractInterface.h"
#include "ACPlayerState.generated.h"


UCLASS()
class ANIMALCRIME_API AACPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job")
	EACCharacterType CharacterType;
};
