// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACGameEnums.h"
#include "Components/ActorComponent.h"
#include "ACUIManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANIMALCRIME_API UACUIManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UACUIManagerComponent();

public:
	void ShowNotification(const FText& Text, ENotificationType NotificationType);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UACNotificationText> NotificationNormalClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UACNotificationText> NotificationWarningClass;
};
