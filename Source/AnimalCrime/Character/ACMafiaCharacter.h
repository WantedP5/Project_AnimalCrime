#pragma once

#include "CoreMinimal.h"
#include "Character/ACCharacter.h"
#include "ACMafiaCharacter.generated.h"

UCLASS()
class ANIMALCRIME_API AACMafiaCharacter : public AACCharacter
{
	GENERATED_BODY()

public:
	AACMafiaCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PostNetInit() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void UpdateCharacterStatusFree();
	void UpdateCharacterStatusRevive();
	
	//!< 상호작용 인터페이스
protected:
	virtual void OnInteract(AACCharacter* ACPlayer, EInteractionKey InKey) override;
	virtual EACInteractorType GetInteractorType() const override;

	UFUNCTION(Server, Reliable)
	void ServerFireHitscan();

public:
/**
	@brief 클라이언트에서 폭탄 설치 가능 영역 표시 여부를 갱신함.
	@param bVisible - true면 설치구역을 보여주고, false면 숨김
**/
	UFUNCTION(Client, Reliable)
	void ClientSetBombAreaVisible(bool bVisible);

	UFUNCTION(Client, Reliable)
	void ClientSetEscapeAreaVisible(bool bVisible);

	virtual EACCharacterType GetCharacterType() override;
protected:

	void ItemDrop() override;

	virtual void ServerItemDrop_Implementation() override;

	UFUNCTION()
	void OnRep_HandBomb();

	virtual void AttackHitCheck() override;
	
	// RayCast를 통해 공격
	void FireHitscan();
	
	// Preojectile 소환
	void FireBullet();

	/**
	 * @brief TimeRate마다 세금을 내는 함수
	 */
	void CalculateTax();

	/**
	 * @brief TimeRate와 기존 Timer 제거 후 재실행
	 * @param InTimeRate 
	 */
	void ChangeTax(float InTimeRate);
	
	// 밀수품 미션 관련 함수
public:
	int32 GetContraband() const { return Constrband; }
	void AddContraband() { ++Constrband; }
	void SubtractContraband() { --Constrband; }
	UFUNCTION(Client, Reliable)
	void Client_ShowGetContraband();

//protected:
//	virtual float GetRequiredHoldTime() const override;

public:
	//!<아이템
	UPROPERTY(ReplicatedUsing = OnRep_HandBomb)
	TObjectPtr<class AACEscapeMissionBomb> HandBomb;

public:
	float GetCurrentHP() const;
	float TickDeltaTime = 1.0f;

protected:
	FTimerHandle TaxTimerHandle;
	float TaxTimeRate = 60.0f;

	// ===== 사운드 추가 =====
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundBase> HitSound;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	int32 Constrband = 0;
};
