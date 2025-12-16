// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/ACInteractableComponent.h"
#include "Character/ACCharacter.h"
#include "AnimalCrime.h"

UACInteractableComponent::UACInteractableComponent()
{
	// 박스 위치 루트에 맞추기
	SetRelativeLocation(FVector::ZeroVector);
	SetCollisionProfileName(TEXT("InteractableCollision"));

	// 부모의 회전을 무시하고 월드 기준 회전 유지
	SetAbsolute(false, true, false);
	BoxMargin = FVector(50.f);
}

void UACInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner() == nullptr)
	{
		return;
	}

	OnComponentBeginOverlap.AddDynamic(this, &UACInteractableComponent::OnInteractOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UACInteractableComponent::OnInteractOverlapEnd);

	// 루트 컴포넌트의 로컬 바운드 가져오기
	if (GetOwner()->GetRootComponent() == nullptr)
	{
		return;
	}

	FBoxSphereBounds RootBounds = GetOwner()->GetRootComponent()->Bounds;

	// BoxExtent 설정 (약간 여유 포함)
	SetBoxExtent(RootBounds.BoxExtent + BoxMargin);
}

void UACInteractableComponent::OnInteractOverlapBegin(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AACCharacter* ACPlayer = Cast<AACCharacter>(OtherActor);
	if (ACPlayer == nullptr)
	{
		return;
	}
	if (GetOwner() == nullptr)
	{
		return;
	}
	if (ACPlayer == GetOwner())
	{
		//UE_LOG(LogSW, Log, TEXT("SELF OverlapBegin"));
		return;
	}

	ACPlayer->AddInteractable(GetOwner());
}

void UACInteractableComponent::OnInteractOverlapEnd(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AACCharacter* ACPlayer = Cast<AACCharacter>(OtherActor);
	if (ACPlayer == nullptr)
	{
		return;
	}
	if (GetOwner() == nullptr)
	{
		return;
	}

	//UE_LOG(LogSW, Log, TEXT("%s OverlapEnd"), *OtherActor->GetName());
	ACPlayer->RemoveInteractable(GetOwner());
}

void UACInteractableComponent::SetMargin(FVector InMargin)
{
	BoxMargin = InMargin;
}
