// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Game/ACGameEnums.h"
#include "ACInteractionDatabase.generated.h"

// 전방 선언
class UACInteractionData;

/**
 * @brief 상호작용 임무 목록을 담는 래퍼 구조체
 *
 * @details
 * TMap의 Value로 TArray를 직접 사용할 수 없어서
 * 구조체로 래핑함 (Unreal 리플렉션 시스템 제약)
 */
USTRUCT(BlueprintType)
struct FInteractionMissionList
{
	GENERATED_BODY()

	// 해당 상호작용에서 가능한 임무들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TArray<UACInteractionData*> Missions;
};

/**
 * @brief 상호작용 검색을 위한 키 구조체
 *
 * @details
 * 주체(경찰/마피아/시민) + 대상(시민/CCTV/블랙마켓 등) 조합으로
 * 어떤 상호작용 임무들이 가능한지 찾는다.
 *
 * 예시:
 * - 경찰 + 시민 → [신분증 검사, 체포]
 * - 마피아 + 블랙마켓 → [무기 구매, 방어구 구매]
 * - 모든 플레이어 + CCTV → [CCTV 보기]
 */
USTRUCT(BlueprintType)
struct FInteractionKey
{
	GENERATED_BODY()

	// 상호작용 주체 (누가)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	EACCharacterType InitiatorType = EACCharacterType::Citizen;

	// 상호작용 대상 타입 (무엇을)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	EACInteractorType TargetType = EACInteractorType::Citizen;

	/**
	 * @brief 두 Enum을 비트 연산으로 하나의 정수 키로 변환
	 * @details InitiatorType을 상위 8비트, TargetType을 하위 8비트에 배치
	 * 예시: Police(0) + Citizen(1) = 0x0001, Mafia(1) + Citizen(1) = 0x0101
	 */
	uint32 ToInt() const
	{
		return (static_cast<uint32>(InitiatorType) << 8) | static_cast<uint32>(TargetType);
	}

	// TMap의 키로 사용하기 위한 비교 연산자 (비트 연산 활용)
	bool operator==(const FInteractionKey& Other) const
	{
		return ToInt() == Other.ToInt();  // 빠른 정수 비교!
	}

	// 해시 함수 (정수 키를 그대로 반환 - 해시 계산 불필요!)
	friend uint32 GetTypeHash(const FInteractionKey& Key)
	{
		return Key.ToInt();
	}
};

/**
 * @brief 중앙 상호작용 데이터베이스
 *
 * @details
 * 모든 캐릭터 타입 x 대상 액터 조합의 상호작용을 한 곳에서 관리한다.
 * 각 액터에 프로퍼티를 추가하는 대신, 이 중앙 DB에서 조회한다.
 *
 * 사용 흐름:
 * 1. 에디터에서 DA_InteractionDatabase 생성
 * 2. InteractionMap에 조합 등록 (경찰+시민, 마피아+블랙마켓 등)
 * 3. 각 조합마다 임무 데이터 애셋들 할당
 * 4. Interact Subsystem에서 이 DB를 참조
 * 5. 런타임에 GetInteractions()로 조회
 *
 * 장점:
 * - 중앙 관리로 유지보수 쉬움
 * - 각 액터에 프로퍼티 추가 불필요
 * - 다중 임무 지원 용이
 * - 데이터 주도 설계
 */
UCLASS(BlueprintType)
class ANIMALCRIME_API UACInteractionDatabase : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * @brief 상호작용 맵: (주체 + 대상) → 임무 목록
	 *
	 * @details
	 * Key: FInteractionKey (InitiatorType + TargetType)
	 * Value: FInteractionMissionList (임무 배열을 담은 구조체)
	 *
	 * 에디터 설정 예시:
	 * [0] Key: {Police, Citizen}
	 *     Value:
	 *       Missions: [DA_PoliceCheckID, DA_PoliceArrest]
	 * [1] Key: {Mafia, BlackMarketDealer}
	 *     Value:
	 *       Missions: [DA_BuyWeapon, DA_BuyArmor]
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Database")
	TMap<FInteractionKey, FInteractionMissionList> InteractionMap;

	/**
	 * @brief 특정 조합에 해당하는 상호작용 임무들을 가져옴
	 *
	 * @param Initiator 주체 캐릭터 타입 (경찰, 마피아, 시민)
	 * @param TargetType 대상 타입 (Citizen, CCTVArea, BlackMarketDealer 등)
	 * @return 해당 조합의 임무 배열 (없으면 빈 배열)
	 *
	 * 사용 예시:
	 * TArray<UACInteractionData*> Missions =
	 *     Database->GetInteractions(EACCharacterType::Police, EACInteractorType::Citizen);
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	TArray<UACInteractionData*> GetInteractions(EACCharacterType Initiator, EACInteractorType TargetType) const
	{
		FInteractionKey Key;
		Key.InitiatorType = Initiator;
		Key.TargetType = TargetType;

		if (const FInteractionMissionList* Found = InteractionMap.Find(Key))
		{
			return Found->Missions;
		}

		// 못 찾으면 빈 배열 반환
		return TArray<UACInteractionData*>();
	}

	/**
	 * @brief 특정 조합에 임무가 존재하는지 확인
	 *
	 * @param Initiator 주체 캐릭터 타입
	 * @param TargetType 대상 타입
	 * @return 임무가 1개 이상 있으면 true
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool HasInteractions(EACCharacterType Initiator, EACInteractorType TargetType) const
	{
		FInteractionKey Key;
		Key.InitiatorType = Initiator;
		Key.TargetType = TargetType;

		const FInteractionMissionList* Found = InteractionMap.Find(Key);
		return Found != nullptr && Found->Missions.Num() > 0;
	}

#if WITH_EDITOR
	/**
	 * @brief 에디터에서 데이터 변경 시 유효성 검사
	 *
	 * @details
	 * - 빈 미션 배열 경고
	 * - nullptr 미션 경고
	 * - 중복 키 검사 (TMap이 자동 처리)
	 */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);

		// 각 항목 검증
		for (const auto& Pair : InteractionMap)
		{
			// 빈 배열 경고
			if (Pair.Value.Missions.Num() == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("[InteractionDatabase] Empty mission array for key: %s + %s"),
					*UEnum::GetValueAsString(Pair.Key.InitiatorType),
					*UEnum::GetValueAsString(Pair.Key.TargetType));
			}

			// nullptr 미션 경고
			for (int32 i = 0; i < Pair.Value.Missions.Num(); ++i)
			{
				if (Pair.Value.Missions[i] == nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("[InteractionDatabase] Null mission at index %d for key: %s + %s"),
						i,
						*UEnum::GetValueAsString(Pair.Key.InitiatorType),
						*UEnum::GetValueAsString(Pair.Key.TargetType));
				}
			}
		}
	}
#endif
};
