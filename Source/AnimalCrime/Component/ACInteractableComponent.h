

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "ACInteractableComponent.generated.h"

/**

    @class   UACInteractableComponent
    @brief   상호작용 가능한 물체에 모두 필요한 상호작용 컴포넌트
    @details ~ 박스 콜리전으로 범위 내에 오버랩 되는 상호작용 가능한 물체를 플레이어의 NearInteractables Set에 추가해준다.
	물체마다 크기가 전부 달라야하므로 만약 캐릭터를 제외한 물체에 이 컴포넌트를 추가한다면 SetMargin을 통해 얼만큼의 Margin을 추가할지 지정해줘야함.
	Margin의 디폴트는 FVector(50.f)임.

**/
UCLASS()
class ANIMALCRIME_API UACInteractableComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UACInteractableComponent();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnInteractOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetMargin(FVector InMargin);

public:
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector BoxMargin;

private:
	
};
