
#include "UI/Common/ACNotificationText.h"
#include "Components/TextBlock.h"

void UACNotificationText::NativeConstruct()
{
	Super::NativeConstruct();

	if (FlyAnim == nullptr)
	{
		return;
	}

	FWidgetAnimationDynamicEvent FinishedEvent;
	FinishedEvent.BindDynamic(this, &UACNotificationText::HandleNotificationFinished);
	BindToAnimationFinished(FlyAnim, FinishedEvent);
	PlayAnimation(FlyAnim);
}

void UACNotificationText::HandleNotificationFinished()
{
	RemoveFromParent();
}

void UACNotificationText::SetNotificationText(const FText& InText)
{
	NotificationText->SetText(InText);
}
