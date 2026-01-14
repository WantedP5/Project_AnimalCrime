
#include "Game/ACUIManagerComponent.h"
#include "UI/Common/ACNotificationText.h"
UACUIManagerComponent::UACUIManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UACNotificationText> NotificationNormalRef(
		TEXT("/Game/Project/UI/Common/WBP_NotificationText.WBP_NotificationText_C"));
	if (NotificationNormalRef.Succeeded())
	{
		NotificationNormalClass = NotificationNormalRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UACNotificationText> NotificationWarningRef(
		TEXT("/Game/Project/UI/Common/WBP_NotificationSpecial.WBP_NotificationSpecial_C"));
	if (NotificationWarningRef.Succeeded())
	{
		NotificationWarningClass = NotificationWarningRef.Class;
	}
}

void UACUIManagerComponent::ShowNotification(const FText& Text, ENotificationType NotificationType)
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (PC == nullptr || PC->IsLocalController() == false)
	{
		return;
	}

	TSubclassOf<UACNotificationText> NotificationClass = nullptr;
	switch (NotificationType)
	{
	case ENotificationType::None:
		NotificationClass = NotificationNormalClass;
		break;
	case ENotificationType::Warning:
		NotificationClass = NotificationWarningClass;
	}

	if (NotificationClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NotificationWidgetClass is nullptr"));
		return;
	}

	UACNotificationText* Widget = CreateWidget<UACNotificationText>(PC, NotificationClass);
	if (Widget == nullptr)
	{
		return;
	}

	Widget->SetNotificationText(Text);
	Widget->AddToViewport();
}