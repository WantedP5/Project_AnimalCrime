// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ACInteractInterface.h"
#include "ACShopArea.generated.h"

UCLASS()
class ANIMALCRIME_API AACShopArea : public AActor, public IACInteractInterface
{
	GENERATED_BODY()
	
public:	
	AACShopArea();

protected:
	virtual void BeginPlay() override;

	virtual bool CanInteract(class AACCharacter* ACPlayer) override;		// 누가 상호작용 가능한지(캐릭터 타입 체크) |
	virtual void OnInteract(class AACCharacter* ACPlayer) override;		// 실제 상호작용 로직(서버에서 실행) |
	virtual FString GetInteractableName() const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TObjectPtr<class UACInteractableComponent> InteractBoxComponent;

	// 상점 UI 위젯 클래스(블루프린트에서 설정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TSubclassOf<class UACShopWidget> ShopWidgetClass;
};
