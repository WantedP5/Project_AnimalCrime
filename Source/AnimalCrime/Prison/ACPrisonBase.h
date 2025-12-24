// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ACInteractInterface.h"
#include "ACPrisonBase.generated.h"

/**

    @class   AACPrisonBase
    @brief   감옥 베이스 클래스.
    @details ~ 투옥되는 경우(배열에 저장)는 두가지이다. 1. 경찰의 체포 2. 자기 발로 걸어들어옴.
	1번같은 경우는 상호작용에 의해서 GameMode -> PrisonManager -> PrisonBase로 오게됨.
	2번은 UpdatePrisoners에서 알아서 처리함.

**/
UCLASS()
class ANIMALCRIME_API AACPrisonBase : public AActor, public IACInteractInterface
{
	GENERATED_BODY()

public:
	AACPrisonBase();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected: //!< 상호작용 인터페이스 구현
	virtual bool CanInteract(class AACCharacter* ACPlayer) override;
	virtual void OnInteract(class AACCharacter* ACPlayer) override;
	virtual float GetRequiredHoldTime() const override;

public:
	/**
		@brief 문 열기 (서버에서 호출)
	**/
	UFUNCTION(BlueprintCallable, Category = "Door")
	void OpenDoor();

	/**
		@brief 문 닫기 (서버에서 호출)
	**/
	UFUNCTION(BlueprintCallable, Category = "Door")
	void CloseDoor();

	bool IsEmpty() const;

	/**
	@brief 투옥(배열에 추가).
	1. 경찰 상호작용으로 불리는 경우와 2. UpdatePrisoners에서 불리는 경우가 있음.
	@param ACChar  - 투옥할 대상 캐릭터
	@param bForced - 경찰이 체포한 것인지의 유무. 스스로 감옥에 들어간 경우는 false이다.
**/
	void Imprison(class AACCharacter* Character, bool bForced);

	/**
		@brief 탈옥(배열에서 제거).
		PrisonAreaBox에서 오버랩이 끝나면 자동으로 제거해줌.
		@param ACChar - 탈옥한 캐릭터
	**/
	void Release(AACCharacter* Character);

protected:
	/**
		@brief 문 상태 변경 시 자동으로 BP_OnDoorStateChanged를 호출.
		서버를 제외한 클라이언트에서 자동으로 불림.
	**/
	UFUNCTION()
	void OnRep_DoorOpen();
	/**
		@brief 문 상태 변경 시 블루프린트에서 구현할 이벤트
		@param bOpen - true면 열림, false면 닫힘
	**/
	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
	void BP_OnDoorStateChanged(bool bOpen);

	FVector GetTeleportLocation() const;

	/**
		@brief bDoorOpen이 false(문 닫기시작)일때, PrisonAreaBox의 오버랩을 확인해서 자기 발로 들어온 마피아 검거.
	**/
	void UpdatePrisoners();

	/**
	@brief PrisonAreaBox 박스 컴포넌트에 오버랩 시작 이벤트가 발생하면 델리게이트로 실행되는 함수.
	@param OverlappedComponent - 이 컴포넌트의 박스 컴포넌트
	@param OtherActor          - 오버랩된 액터
	@param OtherComp           - 오버랩된 액터의 컴포넌트
	@param OtherBodyIndex      - 스켈레탈 메시(Ragdoll)나 파괴 가능한 메시처럼 복잡한 콜리전일 때, 정확히 몇 번째 뼈/조각인지 알려줍니다.
	@param bFromSweep          - 이동 중에 오버랩 했는지의 유무
	@param SweepResult         - 위가 true일때, 부딪힌 정확한 지점, 법선 벡터등의 정보가 담긴 구조체
**/
	//UFUNCTION()
	//void OnPrisonAreaOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**
		@brief PrisonAreaBox 박스 컴포넌트에 오버랩 종료 이벤트가 발생하면 델리게이트로 실행되는 함수.
		@param OverlappedComponent - 이 컴포넌트의 박스 컴포넌트
		@param OtherActor          - 오버랩된 액터
		@param OtherComp           - 오버랩된 액터의 컴포넌트
		@param OtherBodyIndex      - 스켈레탈 메시(Ragdoll)나 파괴 가능한 메시처럼 복잡한 콜리전일 때, 정확히 몇 번째 뼈/조각인지 알려줍니다.
	**/
	UFUNCTION()
	void OnPrisonAreaOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	//!< 상호작용 멤버변수
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TObjectPtr<class UACInteractableComponent> InteractBoxComponent;

	//!< 문 상태
protected:
	UPROPERTY(ReplicatedUsing = OnRep_DoorOpen, BlueprintReadOnly, Category = "Door")
	bool bDoorOpen = false;

	//!< 문이 자동으로 닫히는 시간 (초)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	const float AutoCloseDelay = 5.0f;

	//!< 현재 수감되어있는 모든 캐릭터
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Prison")
	TArray<TObjectPtr<class AACCharacter>> Prisoners;

	//!< 수감 인원 체크를 위한 박스 콜리전
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prison")
	TObjectPtr<class UBoxComponent> PrisonAreaBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prison")
	FVector TeleportOffset;

private:
	FTimerHandle DoorTimerHandle;

};
