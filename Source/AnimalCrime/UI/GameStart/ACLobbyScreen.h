
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACLobbyScreen.generated.h"

UCLASS()
class ANIMALCRIME_API UACLobbyScreen : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
 /**
     @brief 준비된 플레이어와 총 플레이어를 보여주는 UI 갱신 함수
     @param ReadyPlayerNum - 준비된 플레이어
     @param AllPlayerNum   - 총 플레이어
 **/
	UFUNCTION()
	void UpdatePlayerReadyText(int32 ReadyPlayerNum, int32 AllPlayerNum);

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> ReadyPlayerText;
};
