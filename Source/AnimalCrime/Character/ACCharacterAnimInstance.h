
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ACCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Carry")
    bool GetIsCarrying() const { return IsCarrying; }

    UFUNCTION(BlueprintCallable, Category = "Carry")
    void SetIsCarrying(bool bNewCarrying);

    UFUNCTION(BlueprintCallable, Category = "Carry")
    bool GetIsZoomIn() const { return IsZoomIn; }

    UFUNCTION(BlueprintCallable, Category = "Carry")
    void SetIsZoomIn(bool bNewZoomIn);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsCarrying = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool IsZoomIn = false;
};
