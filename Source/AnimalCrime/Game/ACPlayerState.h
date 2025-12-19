
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:

	virtual void CopyProperties(APlayerState* PlayerState) override;
public:
	UFUNCTION()
	void OnRep_CharacterType();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_CharacterType, Category = "Job")
	EACCharacterType CharacterType;
};
