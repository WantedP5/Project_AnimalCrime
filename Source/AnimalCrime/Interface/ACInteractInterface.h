// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Game/ACGameEnums.h"
#include "ACInteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UACInteractInterface : public UInterface
{
	GENERATED_BODY()
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
     @brief 이 액터가 상호작용하는 실제 상호작용 로직. 캐릭터에 따라 기능 구현.
     @param ACPlayer - 이 액터의 InteractComponent와 오버랩된 캐릭터
	 @param InKey - 상호작용에 사용된 키
 **/
	virtual void OnInteract(class AACCharacter* ACPlayer, EInteractionKey InKey) = 0;

	// todo: 상호작용 UI 추가시 활용
	//virtual void OnInteractRangeEnter() = 0;				// Exit | 범위 진입 / 이탈 시(UI 힌트 표시용) |


 /**
     @brief  이 액터의 상호작용 타입을 반환
     @details 중앙 데이터베이스에서 임무를 찾기 위해 사용
     @retval  - 이 액터의 EACInteractorType
 **/
	virtual EACInteractorType GetInteractorType() const = 0;

 /**
     @brief 이 액터의 상호작용 힌트 위젯 컴포넌트를 반환
     @retval  - 위젯 컴포넌트 또는 nullptr
 **/
	virtual class UWidgetComponent* GetInteractionWidget() const { return nullptr; }

 /**
     @brief 상호작용 힌트를 표시
     @param Interactions - 표시할 상호작용 데이터 배열 (이미 필터링됨)
 **/
	virtual void ShowInteractionHints(const TArray<class UACInteractionData*>& Interactions) {}

 /**
     @brief 상호작용 힌트를 숨김
 **/
	virtual void HideInteractionHints() {}

private:



public:
protected:
private:
};
