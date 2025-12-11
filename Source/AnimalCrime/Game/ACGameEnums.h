#pragma once

#include "CoreMinimal.h"
#include "ACGameEnums.generated.h"

UENUM(BlueprintType)
enum class EEscapeState : uint8
{
    GameStart   UMETA(DisplayName = "GameStart"),
    DeliverBomb UMETA(DisplayName = "DeliverBomb"),
    Escape      UMETA(DisplayName = "Escape"),
};