// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ACPrisonManager.generated.h"

/**

    @class   UACPrisonManager
    @brief   MainGameMode에서 사용할 감옥 매니저 클래스
    @details ~ 현재 레밸에 있는 모든 감옥 정보를 가지고있으며, GameMode에 존재하기 때문에 서버만 정보를 알 수 있다.

**/
UCLASS()
class ANIMALCRIME_API UACPrisonManager : public UObject
{
	GENERATED_BODY()

public:
 /**
     @brief 감옥을 등록한다.
	 AACPrisonBase::BeginPlay()에서 호출함.
     @param InPrison - 등록할 감옥.
 **/
	void RegisterPrison(class AACPrisonBase* InPrison);
	//void UnregisterPrison();
 /**
     @brief 경찰 상호작용으로 인해 불리는 투옥 함수.
     @param ACPlayer - 투옥해야할 캐릭터.
 **/
    // todo: 나중에 빈 감옥 중 랜덤으로 투옥?
	void ImprisonCharacter(class AACCharacter* ACPlayer);

protected:
 /**
     @brief  등록되어있는 감옥들 중, 비어있는 감옥을 확인한다.
     @retval  - 비어있는 감옥의 포인터를 반환. 다 차있다면 nullptr 반환.
 **/
	class AACPrisonBase* FindEmptyPrison();


public:
	TArray<class AACPrisonBase*> Prisons;
};
