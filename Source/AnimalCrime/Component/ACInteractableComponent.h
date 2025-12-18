

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "ACInteractableComponent.generated.h"

/**

    @class   UACInteractableComponent
    @brief   상호작용 가능한 물체에 모두 필요한 상호작용 컴포넌트
    @details ~ 박스 콜리전으로 범위 내에 오버랩 되는 상호작용 가능한 물체를 플레이어의 NearInteractables Set에 추가해준다.
	이 컴포넌트 오너의 루트의 크기를 바탕으로 BoxMargin를 더한 값으로 박스 컴포넌트의 크기가 정해진다.
	물체마다 크기가 전부 달라야하므로 만약 캐릭터를 제외한 물체에 이 컴포넌트를 추가한다면 생성자에서 SetMargin을 통해 얼만큼의 Margin을 추가할지 지정해줘야함.
	Margin의 디폴트는 FVector(50.f)임.

**/
UCLASS()
class ANIMALCRIME_API UACInteractableComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UACInteractableComponent();

 /**
     @brief 델리게이트와 박스 컴포넌트의 크기를 지정해줌.
	 주의사항: 만약 루트에 회전이 적용되어있다면, AABB를 염두에 둘것!!(빈 공간이 많이 생길 가능성 있음. 문제가 된다면 원으로 수정)
 **/
	virtual void BeginPlay() override;

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
	UFUNCTION()
	void OnInteractOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

/**
	@brief 박스 컴포넌트에 오버랩 종료 이벤트가 발생하면 델리게이트로 실행되는 함수.
	OtherActor가 플레이어이면 그의 NearInteractables에서 이 컴포넌트를 지닌 오너를 제거한다.
	@param OverlappedComponent - 이 컴포넌트의 박스 컴포넌트
	@param OtherActor          - 오버랩된 액터
	@param OtherComp           - 오버랩된 액터의 컴포넌트
	@param OtherBodyIndex      - 스켈레탈 메시(Ragdoll)나 파괴 가능한 메시처럼 복잡한 콜리전일 때, 정확히 몇 번째 뼈/조각인지 알려줍니다.
**/
	UFUNCTION()
	void OnInteractOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

 /**
     @brief 이 컴포넌트 오너의 루트의 크기를 바탕으로, 박스 컴포넌트의 크기를 더 키우거나 줄일 수 있는 함수.
     @param InMargin - 새로 지정할 박스 컴포넌트의 Margin 크기. 디폴트 크기는 FVector(50.f)으로 설정되어있음.
 **/
	void SetMargin(FVector InMargin);
protected:
private:


public:
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector BoxMargin;

	/*UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint64 ;*/

private:
	
};
