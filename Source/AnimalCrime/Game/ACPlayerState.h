
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Interface/ACInteractInterface.h"
#include "ACPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, NewMoney);

UCLASS()
class ANIMALCRIME_API AACPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public: 
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job")
	EACCharacterType CharacterType;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnMoneyChanged OnMoneyChanged;

	int32 GetMoney() const { return Money; }

	void SetMoney(int32 InMoney);

	UFUNCTION()
	void OnRep_Money();
	
	UPROPERTY(ReplicatedUsing = OnRep_Money)
	int32 Money = 0;
protected:

	
};
