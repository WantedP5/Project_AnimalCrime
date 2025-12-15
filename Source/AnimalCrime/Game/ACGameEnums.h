#pragma once

#include "CoreMinimal.h"
#include "ACGameEnums.generated.h"

//마피아 탈출 임무 현황 열겨험
UENUM(BlueprintType)
enum class EEscapeState : uint8
{
    GameStart   UMETA(DisplayName = "GameStart"),
    DeliverBomb UMETA(DisplayName = "DeliverBomb"),
    Escape      UMETA(DisplayName = "Escape"),
};

//입력매핑 컨텍스트 열거형
UENUM(BlueprintType)
enum class EInputMode : uint8
{
	Sholder		UMETA(DisplayName = "Sholder"),		// 기본
	Settings	UMETA(DisplayName = "Settings"),	// 설정창
};

//설정창 열거형
UENUM(BlueprintType)
enum class ESettingMode : uint8
{
	None			UMETA(DisplayName = "Sholder"),		// 없음
	Default			UMETA(DisplayName = "Sholder"),		// 기본 설정창
	SteamFriendList	UMETA(DisplayName = "Settings"),	// 스팀 친구창
};