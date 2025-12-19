// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ACInteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UACInteractInterface : public UInterface
{
	GENERATED_BODY()
};


/**
    @enum  EACCharacterType
    @brief 캐릭터의 타입 체크에 사용되는 열거형
**/
UENUM(BlueprintType)
enum class EACCharacterType : uint8
{
	Police,
	Mafia,
	Citizen,
	BlackMarketDealer,
	Total
};

/**

    @class   IACInteractInterface
    @brief   상호작용의 구체적인 구현을 위한 인터페이스.
    @details ~ 플레이어와의 상호작용을 위해서라면 반드시 상속받아야한다.

**/
class ANIMALCRIME_API IACInteractInterface
{
	GENERATED_BODY()

public:
 /**
     @brief  이 액터와 상호작용 가능한 캐릭터인지 확인하는 함수
     @param  ACPlayer - 이 액터의 InteractComponent와 오버랩된 캐릭터
     @retval            - 상호작용 가능한 캐릭터의 유무
 **/
	virtual bool CanInteract(class AACCharacter* ACPlayer) = 0;

 /**
     @brief 이 액터가 상호작용하는 실제 상호작용 로직. 캐릭터에 따라 기능 구현.
     @param ACPlayer - 이 액터의 InteractComponent와 오버랩된 캐릭터
 **/
	virtual void OnInteract(class AACCharacter* ACPlayer) = 0;

	// todo: 상호작용 UI 추가시 활용
	//virtual void OnInteractRangeEnter() = 0;				// Exit | 범위 진입 / 이탈 시(UI 힌트 표시용) |

 /**
     @brief  즉시 or 홀드 상호작용 구분을 위한 함수. 또한 홀드가 필요하다면 몇 초간 홀드해야하는지 나타냄.
	 홀드가 필요한 상호작용은 이 함수를 오버라이드해서 필요한 초 수를 반환시켜야함.
     @retval  - 키 홀드가 필요한 상호작용의 필요한 초 수를 반환. 기본값은 0.f(즉발).
	 GetRequiredHoldTime() == 0.f 이면 즉시 상호작용, > 0.f 라면 홀드 해야하는 상호작용임.
 **/
	//todo: TPair<FString, float>으로 반환값 바꿔서 상호작용 여러개여도 연동 가능하도록 수정
	virtual float GetRequiredHoldTime() const;

protected:
/**
	@brief 화면에 붉은 글씨로 플레이어와 충돌 액터를 표시.
	상호작용 확인이 필요시, OnInteraction에 호출함.
	@param ACPlayer - OnInteraction에서 받은 오버랩된 플레이어
	@param InteractedActorName - 상호작용된 액터의 이름
**/
	void ShowInteractDebug(class AACCharacter* ACPlayer, const FString& InteractedActorName);

private:



public:
protected:
private:
};
