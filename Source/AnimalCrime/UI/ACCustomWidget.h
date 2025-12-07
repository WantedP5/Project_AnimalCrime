// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACCustomWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACCustomWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetOwningPawn(APawn* NewOwner) { OwningPawn = NewOwner; }

protected:
	// 현재 위젯을 소유하고 있는 액터 저장용 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn")
	TObjectPtr<APawn> OwningPawn;
};
