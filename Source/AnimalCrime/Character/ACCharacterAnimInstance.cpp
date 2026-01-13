

#include "Character/ACCharacterAnimInstance.h"

void UACCharacterAnimInstance::SetIsCarrying(bool bNewCarrying)
{
    if (IsCarrying == bNewCarrying)
    {
        return; // 변경 없으면 아무 것도 안 함
    }

    IsCarrying = bNewCarrying;
}

void UACCharacterAnimInstance::SetIsZoomIn(bool bNewZoomIn)
{
    if (IsZoomIn == bNewZoomIn)
    {
        return; // 변경 없으면 아무 것도 안 함
    }

    IsZoomIn = bNewZoomIn;
}
