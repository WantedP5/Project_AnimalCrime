// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/ACItemData.h"
#include "ACShopComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponEquippedChanged, class UACItemData*, NewWeapon);

/**
    @class   UACShopComponent
    @brief   상점 관련 데이터 로직을 수행하는 컴포넌트 클래스
    @details 아이템을 구매 후 캐릭터에게 적용했을 때 남에게도 보여야 하므로 서버에게 요청(RPC)
**/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANIMALCRIME_API UACShopComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UACShopComponent();

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    /** @brief 아이템 구매 시도 */
    UFUNCTION(BlueprintCallable, Category = "Shop")
    bool PurchaseItem(class UACItemData* ItemData);

    /** @brief 아이템 착용 */
    UFUNCTION(BlueprintCallable, Category = "Shop")
    void EquipItem(class UACItemData* ItemData);

    /** @brief 구매와 동시에 착용 (의류용) */
    UFUNCTION(BlueprintCallable, Category = "Shop")
    void PurchaseAndEquipItem(class UACItemData* ItemData);

    /** @brief 구매 후 퀵슬롯에 추가 (무기용) */
    UFUNCTION(BlueprintCallable, Category = "Shop")
    void PurchaseAndAddToQuickSlot(class UACItemData* ItemData);

    /** @brief 무기 장착/해제 토글 */
    UFUNCTION(BlueprintCallable, Category = "Shop")
    void ToggleWeaponEquip(class UACItemData* ItemData);

    UFUNCTION()
    void OnRep_EquippedWeapon();

    UPROPERTY(BlueprintAssignable, Category = "Shop")
    FOnWeaponEquippedChanged OnWeaponEquippedChanged;
private:
    void EquipClothing(class UACItemData* ItemData);
    void EquipWeapon(class UACItemData* ItemData);
    void UnequipWeapon();

    UFUNCTION(Server, Reliable)
    void ServerPurchaseAndEquipItem(class UACItemData* ItemData);

    UFUNCTION(Server, Reliable)
    void ServerPurchaseAndAddToQuickSlot(class UACItemData* ItemData);

    UFUNCTION(Server, Reliable)
    void ServerToggleWeaponEquip(class UACItemData* ItemData);

    UFUNCTION(NetMulticast, Reliable)
    void MulticastEquipItem(class UACItemData* ItemData);

    UFUNCTION(NetMulticast, Reliable)
    void MulticastUnequipWeapon();

    UFUNCTION(Client, Reliable)
    void ClientAddToQuickSlot(class UACItemData* ItemData);
public:
    // 착용 중인 아이템들
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Shop")
    TObjectPtr<class UACItemData> EquippedHead;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Shop")
    TObjectPtr<class UACItemData> EquippedFaceAcc;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Shop")
    TObjectPtr<class UACItemData> EquippedTop;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Shop")
    TObjectPtr<class UACItemData> EquippedBottom;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Shop")
    TObjectPtr<class UACItemData> EquippedShoes;

    UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon, BlueprintReadOnly, Category = "Shop", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UACItemData> EquippedWeapon;
};
