#pragma once

#include "CoreMinimal.h"
#include "DestroyableStats.generated.h"

USTRUCT()
struct FDestroyableStats
{
	GENERATED_BODY()
	
	/** 물체의 최대 체력 */
	UPROPERTY(EditAnywhere)
	float MaxHp = 100.0f;
	
	/** 물체의 현재 체력 */
	UPROPERTY(EditAnywhere)
	float CurrentHp = 100.0f;
	
	/** 물체의 방어력 */
	UPROPERTY(EditAnywhere)
	float Armor = 5;
	
	/** 파괴 시 획득 가능한 점수 */
	UPROPERTY(EditAnywhere)
	float ScorePoint = 10;
};
