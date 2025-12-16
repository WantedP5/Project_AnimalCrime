// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ACMainPlayerController.generated.h"

/**
 *
 */
UCLASS()
class ANIMALCRIME_API AACMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AACMainPlayerController();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(Client, Reliable)
	void ShowEscapeUI();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> EscapeScreenClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UUserWidget> EscapeScreen;
	
#pragma region HUD
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UACHUDWidget> ACHUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TObjectPtr<class UACHUDWidget> ACHUDWidget;
#pragma endregion
};
