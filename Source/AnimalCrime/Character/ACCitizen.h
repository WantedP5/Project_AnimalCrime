// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ACInteractInterface.h"
#include "ACCitizen.generated.h"

UCLASS()
class ANIMALCRIME_API AACCitizen : public ACharacter, public IACInteractInterface
{
	GENERATED_BODY()

#pragma region 특수 맴버 함수
public:
	AACCitizen();
#pragma endregion
	
#pragma region 엔진 제공 함수
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
#pragma endregion
	
public:	// 좌표
	FVector GetNextPosition() const;
	
#pragma region 인터랙션 인터페이스
protected:
	virtual bool CanInteract(class AACCharacter* Interactor) override;
	virtual void OnInteract(class AACCharacter* Interactor) override;
	virtual FString GetInteractableName() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TObjectPtr<class UACInteractableComponent> InteractBoxComponent;
#pragma endregion

#pragma region 맴버 변수
public:

protected:

private:
#pragma endregion
};
