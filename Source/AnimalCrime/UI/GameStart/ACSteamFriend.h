
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AdvancedFriendsLibrary.h" 
#include "ACSteamFriend.generated.h"


UCLASS()
class ANIMALCRIME_API UACSteamFriend : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateFriend(const FBPFriendInfo& NewFriendDataInfo);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> PlayerNameText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> AvatarImage;

protected:
	//!< FriendDataInfo 변수
	UPROPERTY(BlueprintReadWrite, Category = "Friend")
	FBPFriendInfo FriendDataInfo;
};
