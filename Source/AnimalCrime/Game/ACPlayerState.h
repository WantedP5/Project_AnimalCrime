
#pragma once

#include "CoreMinimal.h"
#include "ACGameEnums.h"
#include "GameFramework/PlayerState.h"
#include "ACPlayerState.generated.h"

UCLASS()
class ANIMALCRIME_API AACPlayerState : public APlayerState
{
	GENERATED_BODY()	
public: 
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
protected:

	
};
