// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/ACItemData.h"
#include "ACShopComponent.generated.h"

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
	// Sets default values for this component's properties
	UACShopComponent();

protected:
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    /**
       * @brief 아이템 구매 시도
       * @param ItemData 구매할 아이템
       * @return 구매 성공 여부
       */
    UFUNCTION(BlueprintCallable, Category = "Shop")
    bool PurchaseItem(class UACItemData* ItemData);

    /**
     * @brief 아이템 착용
     * @param ItemData 착용할 아이템
     */
    UFUNCTION(BlueprintCallable, Category = "Shop")
    void EquipItem(class UACItemData* ItemData);

    /**
     * @brief 구매와 동시에 착용
     * @param ItemData 구매 후 착용할 아이템
     * @return 성공 여부
     */
    UFUNCTION(BlueprintCallable, Category = "Shop")
    void PurchaseAndEquipItem(class UACItemData* ItemData);


private:
    /**
     * @brief 의류 착용 처리
     */
    void EquipClothing(class UACItemData* ItemData);

    /**
     * @brief 무기 착용 처리
     */
    void EquipWeapon(class UACItemData* ItemData);

private:
    /**
      * @brief 서버에서 구매 및 착용 처리
      */
    UFUNCTION(Server, Reliable)
    void ServerPurchaseAndEquipItem(class UACItemData* ItemData);

    /**
     * @brief 모든 클라이언트에 착용 알림
     */
    UFUNCTION(NetMulticast, Reliable)
    void MulticastEquipItem(class UACItemData* ItemData);

public:


    //// 현재 착용 중인 아이템들 (슬롯별로 관리)
    //UPROPERTY(BlueprintReadOnly, Category = "Shop")
    //TMap<EClothingSlot, TObjectPtr<class UACItemData>> EquippedClothing;

    //// 착용 중인 무기
    //UPROPERTY(BlueprintReadOnly, Category = "Shop")
    //TObjectPtr<class UACItemData> EquippedWeapon;

     // 착용 중인 아이템들 (슬롯별로 개별 변수)
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

    // 착용 중인 무기
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Shop")
    TObjectPtr<class UACItemData> EquippedWeapon;
};
