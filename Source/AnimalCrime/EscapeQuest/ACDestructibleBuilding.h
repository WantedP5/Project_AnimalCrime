
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
	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
 /**
     @brief 건물 파괴 호출 함수
 **/
	UFUNCTION(BlueprintCallable)
	void DestroyBuilding();

 /**
	 @brief 실제 파괴 처리 함수
 **/
	UFUNCTION()
	void OnRep_Destroyed();

	// 함수 선언만 되어있어서 주석처리함
//private:
// /**
//	 @brief 특정 위치에 파괴 스트레인 적용
// **/
//	void ApplyFractureStrain();

public:
	/**
	 * @brief Fade 효과 시작 (클라이언트에서만 호출할 것)
	 */
	UFUNCTION(BlueprintCallable, Category = "Fade")
	void StartFadeLoop();

	/**
	 * @brief Fade 효과 중지 (클라이언트에서만 호출할 것)
	 */
	UFUNCTION(BlueprintCallable, Category = "Fade")
	void StopFade();

protected:
	// Fade 업데이트 함수
	void UpdateFade();

protected:
	// Fade 머티리얼
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TObjectPtr<class UMaterialInterface> FadeMaterial;

	// Fade 상태
	FTimerHandle FadeTimerHandle;
	float CurrentFadeAlpha = 0.0f;
	bool bFadingIn = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	float FadeSpeed = 0.25f;

	// Dynamic Material Instance
	TObjectPtr<UMaterialInstanceDynamic> OverlayDynamicMaterial;

protected:
	// 평소 건물 (메시 / 충돌 / 위치 조정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Destructible", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	// 파괴용 프랙처 메시 (Geometry Collection)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Destructible", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGeometryCollectionComponent> FractureMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UGeometryCollection> FractureCollectionAsset;

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

private:
	FTimerHandle StrainTimerHandle;
};
