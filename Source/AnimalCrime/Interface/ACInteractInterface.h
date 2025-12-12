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
	None,
	Police,
	Mafia,
	Citizen,
	Total
};

/**
 *
 */
class ANIMALCRIME_API IACInteractInterface
{
	GENERATED_BODY()

public:

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

	virtual bool CanInteract(class AACCharacter* Interactor) = 0;		// 누가 상호작용 가능한지(캐릭터 타입 체크) |
	virtual void OnInteract(class AACCharacter* Interactor) = 0;		// 실제 상호작용 로직(서버에서 실행) |

	// 상호작용 대상의 이름을 반환 (디버그 메시지용)
	virtual FString GetInteractableName() const = 0;

	// todo: 상호작용 UI 추가시 활용
	//virtual void OnInteractRangeEnter() = 0;				// Exit | 범위 진입 / 이탈 시(UI 힌트 표시용) |

protected:
	// 공통 디버그 메시지 출력
	void ShowInteractDebug(class AACCharacter* Interactor);
};
