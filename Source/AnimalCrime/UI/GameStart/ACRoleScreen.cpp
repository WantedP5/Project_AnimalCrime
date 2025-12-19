
#include "ACRoleScreen.h"
#include "Game/ACGameEnums.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UACRoleScreen::NativeConstruct()
{
	Super::NativeConstruct();

	if (FadeOutAnim == nullptr)
	{
		return;
	}
}

void UACRoleScreen::HandleFadeOutFinished()
{
	OnFadeOutFinished.Broadcast();
}

void UACRoleScreen::SetRole(EPlayerRole PlayerRole)
{
	switch (PlayerRole)
	{
	case EPlayerRole::Police:
		RoleImage->SetBrushFromTexture(PoliceRoleImage);
		PoliceBackground->SetVisibility(ESlateVisibility::Visible);
		RoleText->SetText(PoliceRoleText);
		break;
	case EPlayerRole::Mafia:
		RoleImage->SetBrushFromTexture(MafiaRoleImage);
		MafiaBackground->SetVisibility(ESlateVisibility::Visible);
		RoleText->SetText(MafiaRoleText);
		break;
	default:
		break;
	}

	FWidgetAnimationDynamicEvent FinishedEvent;
	FinishedEvent.BindDynamic(this, &UACRoleScreen::HandleFadeOutFinished);
	BindToAnimationFinished(FadeOutAnim, FinishedEvent);
	PlayAnimation(FadeOutAnim);
}
