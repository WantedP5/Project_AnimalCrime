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
	Spectator	UMETA(DisplayName = "Spectator"),	// 관전자
};

//설정창 열거형
UENUM(BlueprintType)
enum class ESettingMode : uint8
{
	None			UMETA(DisplayName = "Sholder"),		// 없음
	Default			UMETA(DisplayName = "Sholder"),		// 기본 설정창
	SteamFriendList	UMETA(DisplayName = "Settings"),	// 스팀 친구창
	Interact		UMETA(DisplayName = "Interact"),    // 인터랙트
};

//플레이어 역할
UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
	Police			UMETA(DisplayName = "Police"),		// 경찰
	Mafia			UMETA(DisplayName = "Mafia"),		// 마피아
	Observer		UMETA(DisplayName = "Observer"),	// 관전자
};

//보이스 모드
UENUM(BlueprintType)
enum class EVoiceMode : uint8
{
	None			UMETA(DisplayName = "None"),		// None
	Positional		UMETA(DisplayName = "Positional"),	// Positional
	Radio			UMETA(DisplayName = "Radio"),		// Radio
};

// 캐릭터 상태
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	None,
	Free,		
	OnDamage,
	Interact,
	Angry,
	Stun,
	Prison,
	MAX_COUNT
};

// 캐릭터 위치 상태
UENUM(BlueprintType)
enum class ECharacterLocation : uint8
{
	None,
	Prison,
	Escape
};