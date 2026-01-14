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


	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayHitEffect(float Duration);

public:
/**
	@brief 클라이언트에서 폭탄 설치 가능 영역 표시 여부를 갱신함. 버리기 UI도 표시
	@param bVisible - true면 설치구역을 보여주고, false면 숨김. true면 버리기 UI 표시
**/
	UFUNCTION(Client, Reliable)
	void ClientSetBombHeld(bool bIsHolding);

	UFUNCTION(Client, Reliable)
	void ClientSetEscapeAreaVisible(bool bVisible);

	virtual EACCharacterType GetCharacterType() const override;
public:

	void ItemDrop() override;

	virtual void ServerItemDrop_Implementation() override;

	UFUNCTION()
	void OnRep_HandBomb();

	virtual void AttackHitCheck(int32 DamageAmount) override;
	
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
	void AddContraband();
	void SubtractContraband();

	// ===== 추가: 무전기 관련 함수 =====
	bool HasWalkyTalky() const { return bHasWalkyTalky; }
	void SetWalkyTalky(bool bInHasWalkyTalky);

public:
	UFUNCTION()
	void MoveToEscapeWaitingLocation();
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
	// ===== 수정: Replicated + RepNotify 추가 =====

		/**
		 * @brief 밀수품 소지 개수 (Replicated)
		 */
	UPROPERTY(ReplicatedUsing = OnRep_Contraband, EditAnywhere, BlueprintReadWrite, Category = "Mission")
	int32 Constrband = 0;

	/**
	 * @brief 무전기 소지 여부 (Replicated)
	 */
	UPROPERTY(ReplicatedUsing = OnRep_HasWalkyTalky, BlueprintReadOnly, Category = "Mission")
	bool bHasWalkyTalky = false;

	// ===== RepNotify 함수 =====

	UFUNCTION()
	void OnRep_Contraband();

	UFUNCTION()
	void OnRep_HasWalkyTalky();
	
public:
	void ExcuteEscape();
	
	UFUNCTION(Server, Reliable)
	void ServerEscape();
	void PerformEscape();
	
	
	UPROPERTY(Replicated)
	int32 EscapeCount = 3;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float DamaeRate = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float OnHitEffectRate = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float StunRate = 10.0f;
protected:
	
	// UPROPERTY()
	// TObjectPtr<UDataAsset> 
};
