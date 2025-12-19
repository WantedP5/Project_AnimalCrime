#pragma once

#include "CoreMinimal.h"
#include "MoneyData.generated.h"

UENUM()
enum class EMoneyType : uint8
{
	None,
	MoneyPoliceType,
	MoneyMafiaType,
	MoneyCitizenType,
};

USTRUCT(BlueprintType)
struct FMoneyData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Money = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMoneyType MoneyType;
};
