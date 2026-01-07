
#pragma once

#include "CoreMinimal.h"
#include "Game/ACGameEnums.h"
#include "GameFramework/Actor.h"
#include "ACDestructibleBuilding.generated.h"

/**
	@struct FDestroyParticleInfo
	@brief 파괴된 건물에서 재생할 파티클 정보
**/
USTRUCT(BlueprintType)
struct FDestroyParticleInfo
{
	GENERATED_BODY()

	/** 파괴 종료 파티클 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UParticleSystem> Particle = nullptr;

	/** 파티클 위치 / 회전 / 스케일 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform Transform;

	/** 개별 스케일 배수 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ScaleMultiplier = 1.f;
};

UCLASS()
class ANIMALCRIME_API AACDestructibleBuilding : public AActor
{
	GENERATED_BODY()

public:
	AACDestructibleBuilding();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UFUNCTION(BlueprintCallable)
	void DestroyBuilding();

	UFUNCTION()
	void OnRep_Destroyed();

protected:
	// 평소 건물 (메시 / 충돌 / 위치 조정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Destructible", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	// 파괴용 프랙처 메시 (Geometry Collection)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Destructible", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGeometryCollectionComponent> FractureMesh;

	// 붕괴 이펙트 (Cascade)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UParticleSystem> DestroyEffect;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spot")
	ESpot BombSpot = ESpot::Bank;

	//!< 파괴 여부
	UPROPERTY(ReplicatedUsing = OnRep_Destroyed)
	bool bDestroyed = false;

protected:
	/** 파괴 스트레인 적용 위치 오프셋 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fracture")
	FVector StrainLocationOffset = FVector(0.f, 0.f, 300.f);

protected:
	/** 파괴 종료 시 재생할 파티클들 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TArray<FDestroyParticleInfo> EndDestroyParticles;
};
