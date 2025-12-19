
#pragma once

#include "CoreMinimal.h"
#include "Game/ACGameEnums.h"
#include "Blueprint/UserWidget.h"
#include "ACRoleScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRoleScreenFadeFinishedDelegate);
UCLASS()
class ANIMALCRIME_API UACRoleScreen : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void HandleFadeOutFinished();

public:
	void SetRole(EPlayerRole PlayerRole);

public:
	UPROPERTY(BlueprintAssignable)
	FRoleScreenFadeFinishedDelegate OnFadeOutFinished;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> PoliceBackground;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> MafiaBackground;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> RoleImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> RoleText;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FadeOutAnim;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Role")
	TObjectPtr<UTexture2D> MafiaRoleImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Role")
	TObjectPtr<UTexture2D> PoliceRoleImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Role")
	FText MafiaRoleText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Role")
	FText PoliceRoleText;
};
