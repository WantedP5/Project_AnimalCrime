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

	OnComponentBeginOverlap.AddDynamic(this, &UACInteractableComponent::OnInteractOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UACInteractableComponent::OnInteractOverlapEnd);

	if (GetOwner() == nullptr)
	{
		return;
	}

	// 루트 컴포넌트의 로컬 바운드 가져오기
	FBoxSphereBounds RootBounds = GetOwner()->GetRootComponent()->Bounds;

	// BoxExtent 설정 (약간 여유 포함)
	SetBoxExtent(RootBounds.BoxExtent + BoxMargin);

}

/**
	@brief 박스 컴포넌트에 오버랩 시작 이벤트가 발생하면 델리게이트로 실행되는 함수.
	OtherActor가 플레이어이면 그의 NearInteractables에 이 컴포넌트를 지닌 오너를 추가한다.
	@param OverlappedComponent - 이 컴포넌트의 박스 컴포넌트
	@param OtherActor          - 오버랩된 액터
	@param OtherComp           - 오버랩된 액터의 컴포넌트
	@param OtherBodyIndex      - 스켈레탈 메시(Ragdoll)나 파괴 가능한 메시처럼 복잡한 콜리전일 때, 정확히 몇 번째 뼈/조각인지 알려줍니다.
	@param bFromSweep          - 이동 중에 오버랩 했는지의 유무
	@param SweepResult         - 위가 true일때, 부딪힌 정확한 지점, 법선 벡터등의 정보가 담긴 구조체
**/
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

/**
	@brief 박스 컴포넌트에 오버랩 종료 이벤트가 발생하면 델리게이트로 실행되는 함수.
	OtherActor가 플레이어이면 그의 NearInteractables에서 이 컴포넌트를 지닌 오너를 제거한다.
	@param OverlappedComponent - 이 컴포넌트의 박스 컴포넌트
	@param OtherActor          - 오버랩된 액터
	@param OtherComp           - 오버랩된 액터의 컴포넌트
	@param OtherBodyIndex      - 스켈레탈 메시(Ragdoll)나 파괴 가능한 메시처럼 복잡한 콜리전일 때, 정확히 몇 번째 뼈/조각인지 알려줍니다.
**/
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
