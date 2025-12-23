// Fill out your copyright notice in the Description page of Project Settings.


#include "ACDestroyableStatComponent.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UACDestroyableStatComponent::UACDestroyableStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UACDestroyableStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UACDestroyableStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UACDestroyableStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UACDestroyableStatComponent, DestroyableStats);
}

void UACDestroyableStatComponent::OnDamaged(float DamageAmount)
{
	float ObjectHp = GetCurrentHp(); 
	
	// 방어력 적용(음수의 경우 데미지가 더 강해진다)
	DamageAmount = FMath::Max(DamageAmount - GetArmor(), 0);
	
	UE_LOG(LogTemp, Log, TEXT("Damage:%f %f"), DamageAmount, GetArmor());
	
	// 실제 체력 결과
	ObjectHp = FMath::Clamp(ObjectHp - DamageAmount, 0, GetMaxHp());
	SetCurrentHp(ObjectHp);
}
