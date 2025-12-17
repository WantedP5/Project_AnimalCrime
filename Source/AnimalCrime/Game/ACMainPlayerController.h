// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Game/ACGameEnums.h"
#include "ACMainPlayerController.generated.h"

/**
 *
 */
UCLASS()
class ANIMALCRIME_API AACMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AACMainPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// ===== 입력 처리 핸들러 =====
protected:
	void HandleMove(const struct FInputActionValue& Value);
	void HandleLook(const struct FInputActionValue& Value);
	void HandleJump(const struct FInputActionValue& Value);
	void HandleStopJumping(const struct FInputActionValue& Value);
	void HandleInteract(const struct FInputActionValue& Value);
	void HandleItemDrop(const struct FInputActionValue& Value);
	void HandleAttack(const struct FInputActionValue& Value);
	void HandleSettingsClose(const struct FInputActionValue& Value);

public:
	/**
		@brief 입력 모드 변경 (기본 입력 ↔ 설정 메뉴 입력)
		@param NewMode - 새로운 입력 모드
	**/
	void ChangeInputMode(EInputMode NewMode);

public:
	UFUNCTION(Client, Reliable)
	void ShowEscapeUI();

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
    UFUNCTION()
    void CloseShop();

protected:
    /**
            @brief 상점용 카메라로 전환
    **/
    void SetShopCamera();

    /**
            @brief 원래 카메라로 복원
    **/
    void RestoreOriginalCamera();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> EscapeScreenClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UUserWidget> EscapeScreen;

	// ===== 입력 관련 =====
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> ItemDropAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> MeleeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> SettingsMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> SettingsCloseAction;


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
	
#pragma region HUD
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UACHUDWidget> ACHUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TObjectPtr<class UACHUDWidget> ACHUDWidget;
#pragma endregion
};
