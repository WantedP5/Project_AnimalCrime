// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ACInteractInterface.h"
#include "ACShopArea.generated.h"

/**

    @class   AACShopArea
    @brief   상점 구역에서 인터랙트를 하게 해주는 클래스
    @details ACInteractInterface를 상속받아 오버라이드로 구현

**/
UCLASS()
class ANIMALCRIME_API AACShopArea : public AActor, public IACInteractInterface
{
	GENERATED_BODY()
	
public:	
	AACShopArea();

protected:
	virtual void BeginPlay() override;

 /**
     @brief  누가 상호작용 가능한지 
     @param  ACPlayer - 타입 체크를 위한 캐릭터 가져오기
     @retval          - 마피아 또는 경찰(현재는 모두 가능하도록 바로 return true)
 **/
	virtual bool CanInteract(class AACCharacter* ACPlayer) override;

 /**
     @brief 실제 상호작용 로직(서버에서 실행)
     @param ACPlayer - 타입 체크를 위한 캐릭터 가져오기
 **/
	virtual void OnInteract(class AACCharacter* ACPlayer) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TObjectPtr<class UACInteractableComponent> InteractBoxComponent;

	// 상점 UI 위젯 클래스(블루프린트에서 설정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TSubclassOf<class UACShopWidget> ShopWidgetClass;
};
