// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "ACInteractableComponent.generated.h"

/**
 * 상호작용 대상에 부착하는 컴포넌트
 * 플레이어가 범위에 들어오면 감지
 */
UCLASS()
class ANIMALCRIME_API UACInteractableComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnInteractOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
