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
 *
 */
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

 /**
     @brief  이 액터의 이름을 반환(디버그 메시지용)
     @retval  - 이름을 FString으로 반환
 **/
	virtual FString GetInteractableName() const = 0;

	// todo: 상호작용 UI 추가시 활용
	//virtual void OnInteractRangeEnter() = 0;				// Exit | 범위 진입 / 이탈 시(UI 힌트 표시용) |

protected:
	void ShowInteractDebug(class AACCharacter* ACPlayer);
};
