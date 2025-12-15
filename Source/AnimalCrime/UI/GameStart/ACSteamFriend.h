
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AdvancedFriendsLibrary.h" 
#include "ACSteamFriend.generated.h"


UCLASS()
class ANIMALCRIME_API UACSteamFriend : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateFriend(const FBPFriendInfo& NewFriendDataInfo);

protected:
	UFUNCTION()
	void OnInviteButtonClicked();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> PlayerNameText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> AvatarImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> InviteButton;

protected:
	//!< FriendDataInfo 변수
	UPROPERTY(BlueprintReadWrite, Category = "Friend")
	FBPFriendInfo FriendDataInfo;
};
