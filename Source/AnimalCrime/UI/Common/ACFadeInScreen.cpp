
#include "ACFadeInScreen.h"

void UACFadeInScreen::NativeConstruct()
{
    Super::NativeConstruct();

    if (FadeInAnim==nullptr)
    {
        return;
    }

    FWidgetAnimationDynamicEvent FinishedEvent;
    FinishedEvent.BindDynamic(this, &UACFadeInScreen::HandleFadeInFinished);
    BindToAnimationFinished(FadeInAnim, FinishedEvent);
    PlayAnimation(FadeInAnim);
}

void UACFadeInScreen::HandleFadeInFinished()
{
    OnFadeInFinished.Broadcast();
}
