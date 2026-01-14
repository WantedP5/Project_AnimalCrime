// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACPlayerControllerBase.h"
#include "Game/ACGameEnums.h"
#include "ACMainPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWalkyTalkyChanged, bool, bHasWalkyTalky);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnContrabandChanged, bool, bHasContraband);

/**
 *
 */
UCLASS()
class ANIMALCRIME_API AACMainPlayerController : public AACPlayerControllerBase
{
	GENERATED_BODY()

public:
	AACMainPlayerController();

protected:
	virtual void PostInitializeComponents() override;
	virtual void PostNetInit() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupInputComponent() override;

	virtual void OnRep_PlayerState() override;
	// ===== 입력 처리 핸들러 =====
protected:
	virtual void HandleAttack(const struct FInputActionValue& Value) override;
	void HandleSpectatorChange(const struct FInputActionValue& Value);
	void HandleDash(const struct FInputActionValue& Value);
	void HandleEscape(const struct FInputActionValue& Value);

	// ===== 퀵슬롯 관련 (하나의 핸들러로 통합) =====
	void HandleQuickSlot(const struct FInputActionValue& Value);

	// ===== 핸드폰 관련 =====
	void HandlePhone(const struct FInputActionValue& Value);

	// ======스킬 체크 전 상태 체크 함수========
	// bool CanUseSkill() const; 플레이어 컨트롤러 베이스로 이전
	bool CanUseEscapeSkill() const;
public:
	void TimeUp();
	void TimeDown();

public:
	/**
		@brief 입력 모드 변경 (기본 입력 ↔ 설정 메뉴 입력)
		@param NewMode - 새로운 입력 모드
	**/
	virtual void ChangeInputMode(EInputMode NewMode) override;


	// ===== 상점 관련 =====
public:
	/**
			@brief  상점 위젯 토글 (서버 → 클라이언트 명령)
			@param WidgetClass - 상점 위젯
	**/
	UFUNCTION(Client, Reliable)
	void ClientToggleShopWidget(TSubclassOf<class UACShopWidget> WidgetClass);

	/**
			@brief  상점 닫기 (위젯의 CloseButton이나 다른 곳에서 호출)
	**/
	UFUNCTION(Client, Reliable)
	void CloseShop();

	/**
		   @brief  CCTV 닫기 (위젯의 CloseButton이나 다른 곳에서 호출)
   **/
	UFUNCTION(Client, Reliable)
	void CloseCCTV();

protected:
	/**
			@brief 상점용 카메라로 전환
	**/
	void SetShopCamera();

	/**
			@brief 원래 카메라로 복원
	**/
	void RestoreOriginalCamera();

	// ===== CCTV 관련 =====
public:
	/**
		  @brief  CCTV 위젯 토글 (서버 → 클라이언트 명령)
		  @param WidgetClass - CCTV 위젯
  **/
	UFUNCTION(Client, Reliable)
	void ClientToggleCCTVWidget(TSubclassOf<class UACCCTVWidget> WidgetClass);

public:
	void ShowInteractProgress(const FString& InteractionName);
	void UpdateInteractProgress(float Progress);
	void HideInteractProgress();

	// ===== 핸드폰 관련 =====
public:
	UFUNCTION()
	void ClosePhone();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Phone")
	TSubclassOf<class UUserWidget> PhoneWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<class UUserWidget> CurrentPhoneWidget;


public:
	// ===== 추가: 바운드 아이템 관련 함수 =====

	/**
	 * @brief 무전기 획득 (로컬)
	 */
	UFUNCTION(BlueprintCallable, Category = "BoundItem")
	void SetHasWalkyTalky(bool bInHasWalkyTalky);

	/**
	 * @brief 밀수품 획득 (로컬)
	 */
	UFUNCTION(BlueprintCallable, Category = "BoundItem")
	void SetHasContraband(bool bInHasContraband);

	/**
	 * @brief 무전기 소지 여부 확인
	 */
	UFUNCTION(BlueprintPure, Category = "BoundItem")
	bool HasWalkyTalky() const { return bHasWalkyTalky; }

	/**
	 * @brief 밀수품 소지 여부 확인
	 */
	UFUNCTION(BlueprintPure, Category = "BoundItem")
	bool HasContraband() const { return bHasContraband; }

protected:
	// ===== 추가: 로컬 바운드 아이템 정보 (Replicated 아님!) =====

	//!< 무전기 소지 여부 (로컬)
	UPROPERTY(BlueprintReadOnly, Category = "BoundItem")
	bool bHasWalkyTalky = false;

	//!< 밀수품 소지 여부 (로컬)
	UPROPERTY(BlueprintReadOnly, Category = "BoundItem")
	bool bHasContraband = false;

public:
	// ===== 추가: 델리게이트 =====

	UPROPERTY(BlueprintAssignable, Category = "BoundItem")
	FOnWalkyTalkyChanged OnWalkyTalkyChanged;

	UPROPERTY(BlueprintAssignable, Category = "BoundItem")
	FOnContrabandChanged OnContrabandChanged;



	// ===== 게임 시작시 역할 UI=====
protected:
	UFUNCTION()
	void OnRoleFadeInFinished();

	void ScreenSetRole();

	// ===== 관전 =====
public:

	/**
		@brief 탈출 성공 UI를 띄우고 IMC를 관전용으로 바꾸는 함수
	**/
	UFUNCTION(Client, Reliable)
	void ClientOnEscapeSuccess();

	/**
		@brief 폰을 숨기고 관전으로 바꾸는 함수
	**/
	UFUNCTION(Server, Reliable)
	void ServerStartSpectateOtherPlayer();

	/**
		@brief 다음 관전 대상으로 전환
	**/
	UFUNCTION(Server, Reliable)
	void ServerSwitchToNextSpectateTarget();

	/**
		@brief 관전 대상이 제거가 되었으면 관전 대상을 바꾼다.
		@param RemovedPawn - 제거된 폰
	**/
	UFUNCTION(Client, Reliable)
	void ClientNotifySpectateTargetRemoved(APawn* RemovedPawn);

	/**
	  @brief 관전 대상이 변경되었을 때 UI 업데이트
	  @param NewTargetPS - 새 관전 대상의 PlayerState
  **/
	UFUNCTION(Client, Reliable)
	void ClientOnSpectateTargetChanged(class AACPlayerState* NewTargetPS);

	/**
		@brief 알림 메시지 표시용 함수
		@param Text - 함림 텍스트
	**/
	UFUNCTION(BlueprintCallable)
	void ShowNotification(const FText& Text, ENotificationType NotificationType);

	UFUNCTION(Client, Reliable)
	void Client_ShowNotification(const FText& Message, ENotificationType NotificationType);

	UFUNCTION(Client, Reliable)
	void Client_ShowGameResult(EGameEndType GameEndType);

	UFUNCTION()
	void ZoomIn();

	UFUNCTION()
	void ZoomOut();

	UPROPERTY(ReplicatedUsing=OnRep_Zoom)
	uint8 bZoomFlag : 1 = false;
	
	UFUNCTION(Server, Reliable)
	void Server_Zoom(bool NewZoomFlag);
	
	UFUNCTION()
	void OnRep_Zoom();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UACSpectatorScreen> SpectatorScreenClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UACSpectatorScreen> SpectatorScreen;

	// ===== 입력 관련 =====
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> DashAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> EscapeAction;

	// ===== 게임 시간 관련 테스트 =========
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> TimerUpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> TimerDownAction;
	
	// ===== 총기 관련 테스트 =========
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> ZoomAction;

	// ===== 퀵슬롯 관련 (하나만) =====
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> QuickSlotAction;

	// ===== 핸드폰 관련 =====
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> PhoneAction;

	// ===== 관전자 키 입력 =====
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> SpectatorMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> SpectatorChangeAction;

	// ===== 상점 관련 =====
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Shop")
	TSubclassOf<class UACShopWidget> ShopWidgetClass;

private:
	// 현재 열려있는 상점 위젯
	UPROPERTY()
	TObjectPtr<class UACShopWidget> CurrentShopWidget;

	// 상점 열기 전 카메라 상태 저장
	FRotator SavedControlRotation;
	float SavedTargetArmLength;
	FVector SavedTargetOffset;
	FVector SavedRelativeLocation;
	FRotator SavedRelativeRotation;
	bool bSavedUsePawnControlRotation;

	uint8 bShopCameraActive : 1 = false;

	// ===== CCTV 관련 =====
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|CCTV")
	TSubclassOf<class UACCCTVWidget> CCTVWidgetClass;

private:
	// 현재 열려있는 CCTV 위젯
	UPROPERTY()
	TObjectPtr<class UACCCTVWidget> CurrentCCTVWidget;

#pragma region HUD
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UACHUDWidget> ACHUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TObjectPtr<class UACHUDWidget> ACHUDWidget;
#pragma endregion


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
	TSubclassOf<class UACRoleScreen> RoleScreenClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role")
	TObjectPtr<class UACRoleScreen> RoleScreen;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Result")
	TSubclassOf<class UACGameResultScreen> GameResultScreenClass;

protected:
	//!< UI 매니저 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UACUIManagerComponent> UIManager;

protected:
	//!< 관전자 Index
	int32 CurrentSpectateIndex = INDEX_NONE;
public:
	void UpdateAmmoUI(int32 Ammo);
};
