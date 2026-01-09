// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Game/ACGameEnums.h"
#include "ACInteractionData.generated.h"

/**
 * @brief 상호작용 데이터 애셋 (UDataAsset 기반)
 * Asset Manager를 통한 중앙 관리 및 비동기 로딩 지원
 *
 * 향후 확장: 한 캐릭터에 여러 임무를 할당하여 다중 선택 UI 지원 가능
 */
UCLASS(BlueprintType)
class ANIMALCRIME_API UACInteractionData : public UDataAsset
{
	GENERATED_BODY()

public:
	// === 임무 정보 ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText InteractionName;  // "시민 신분증 검사", "블랙마켓 거래"

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	EInteractionKey AssignedKey = EInteractionKey::E;  // 'E', 'R', 'T' (다중 임무 시 사용)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	TArray<ECharacterState> InteractorState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	TArray<ECharacterState> TargetState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ECharacterLocation InteractorLocation = ECharacterLocation::None;

	// === UI 정보 ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float HoldDuration = 0.0f;  // 홀드 시간 (0 = 즉시)

	//todo: needed?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FLinearColor ProgressColor = FLinearColor::Green;

	// === 애니메이션 ===
	// 직접 참조로 타입 안정성 확보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* InitiatorMontage;  // 주체 애니메이션 (경찰)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* TargetMontage;  // 대상 애니메이션 (시민)

	//todo: needed?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FName MontageSectionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bIsCharacterInteraction = false;  // 캐릭터면 서로 마주보기

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float FaceToFaceSpeed = 10.0f;

	// === 효과 ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* Sound;  // 직접 참조

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* Particle;  // 직접 참조

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	FVector ParticleOffset = FVector::ZeroVector;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);

		// 에디터에서 프로퍼티 변경 시 유효성 검사
		if (HoldDuration < 0.0f)
		{
			HoldDuration = 0.0f;
		}
	}
#endif
};
