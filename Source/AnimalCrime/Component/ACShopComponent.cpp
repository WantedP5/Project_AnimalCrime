// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/ACShopComponent.h"
#include "Character/ACCharacter.h"
#include "AnimalCrime.h"
#include "Net/UnrealNetwork.h"
#include "UI/HUD/ACQuickSlotWidget.h"
#include "UI/Score/ACHUDWidget.h"
#include "Game/ACMainPlayerController.h"

UACShopComponent::UACShopComponent()
{
    SetIsReplicatedByDefault(true);
}

void UACShopComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UACShopComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UACShopComponent, EquippedHead);
    DOREPLIFETIME(UACShopComponent, EquippedFaceAcc);
    DOREPLIFETIME(UACShopComponent, EquippedTop);
    DOREPLIFETIME(UACShopComponent, EquippedBottom);
    DOREPLIFETIME(UACShopComponent, EquippedShoes);
    DOREPLIFETIME(UACShopComponent, EquippedWeapon);
}

bool UACShopComponent::PurchaseItem(UACItemData* ItemData)
{
    if (ItemData == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("PurchaseItem: ItemData is null"));
        return false;
    }

    // TODO: 돈 체크 및 차감 로직 추가
    return true;
}

void UACShopComponent::EquipItem(UACItemData* ItemData)
{
    if (ItemData == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("EquipItem: ItemData is null"));
        return;
    }

    switch (ItemData->ItemType)
    {
    case EItemType::Clothing:
        EquipClothing(ItemData);
        break;
    case EItemType::Weapon:
        EquipWeapon(ItemData);
        break;
    }
}

void UACShopComponent::PurchaseAndEquipItem(UACItemData* ItemData)
{
    if (ItemData == nullptr) return;

    ServerPurchaseAndEquipItem(ItemData);
}

void UACShopComponent::PurchaseAndAddToQuickSlot(UACItemData* ItemData)
{
    if (ItemData == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("PurchaseAndAddToQuickSlot: ItemData is null"));
        return;
    }

    AACCharacter* Character = Cast<AACCharacter>(GetOwner());
    if (Character == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("PurchaseAndAddToQuickSlot: Owner is not AACCharacter"));
        return;
    }

    AACMainPlayerController* PC = Character->GetController<AACMainPlayerController>();
    if (PC == nullptr || PC->ACHUDWidget == nullptr || PC->ACHUDWidget->WBP_QuickSlot == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("PurchaseAndAddToQuickSlot: QuickSlot widget is null"));
        return;
    }

    if (PC->ACHUDWidget->WBP_QuickSlot->IsFull())
    {
        UE_LOG(LogHG, Warning, TEXT("QuickSlot is full! Cannot purchase item: %s"), *ItemData->ItemName.ToString());
        return;
    }

    if (PurchaseItem(ItemData))
    {
        if (PC->ACHUDWidget->WBP_QuickSlot->TryAddItem(ItemData))
        {
            UE_LOG(LogHG, Log, TEXT("Item purchased and added to quickslot: %s"), *ItemData->ItemName.ToString());
        }
    }
}

void UACShopComponent::ToggleWeaponEquip(UACItemData* ItemData)
{
    if (ItemData == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("ToggleWeaponEquip: ItemData is null"));
        return;
    }

    ServerToggleWeaponEquip(ItemData);
}

void UACShopComponent::EquipClothing(UACItemData* ItemData)
{
    AACCharacter* Character = Cast<AACCharacter>(GetOwner());
    if (Character == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("EquipClothing: Owner is not AACCharacter"));
        return;
    }

    if (ItemData->ClothingMesh == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("EquipClothing: ClothingMesh is null for %s"), *ItemData->ItemName.ToString());
        return;
    }

    USkeletalMeshComponent* TargetMeshComponent = nullptr;

    switch (ItemData->ClothingSlot)
    {
    case EClothingSlot::Head:
        TargetMeshComponent = Character->GetHeadMesh();
        if (GetOwner()->HasAuthority()) EquippedHead = ItemData;
        break;
    case EClothingSlot::FaceAcc:
        TargetMeshComponent = Character->GetFaceAccMesh();
        if (GetOwner()->HasAuthority()) EquippedFaceAcc = ItemData;
        break;
    case EClothingSlot::Top:
        TargetMeshComponent = Character->GetTopMesh();
        if (GetOwner()->HasAuthority()) EquippedTop = ItemData;
        break;
    case EClothingSlot::Bottom:
        TargetMeshComponent = Character->GetBottomMesh();
        if (GetOwner()->HasAuthority()) EquippedBottom = ItemData;
        break;
    case EClothingSlot::Shoes:
        TargetMeshComponent = Character->GetShoesMesh();
        if (GetOwner()->HasAuthority()) EquippedShoes = ItemData;
        break;
    default:
        UE_LOG(LogHG, Warning, TEXT("Unsupported ClothingSlot"));
        return;
    }

    if (TargetMeshComponent == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("TargetMeshComponent is null for slot"));
        return;
    }

    TargetMeshComponent->SetSkeletalMesh(ItemData->ClothingMesh);
    TargetMeshComponent->SetLeaderPoseComponent(Character->GetMesh());

    UE_LOG(LogHG, Log, TEXT("Equipped clothing: %s to slot %d"), *ItemData->ItemName.ToString(), (int32)ItemData->ClothingSlot);
}

void UACShopComponent::EquipWeapon(UACItemData* ItemData)
{
    AACCharacter* Character = Cast<AACCharacter>(GetOwner());
    if (Character == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("EquipWeapon: Owner is not AACCharacter"));
        return;
    }

    if (ItemData->EquipmentMesh == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("EquipWeapon: EquipmentMesh is null"));
        return;
    }

    // 기존 무기 제거
    if (EquippedWeapon != nullptr)
    {
        TArray<UStaticMeshComponent*> StaticMeshComponents;
        Character->GetComponents<UStaticMeshComponent>(StaticMeshComponents);

        for (UStaticMeshComponent* MeshComp : StaticMeshComponents)
        {
            if (MeshComp->GetAttachSocketName() == EquippedWeapon->AttachSocketName)
            {
                MeshComp->DestroyComponent();
                break;
            }
        }
    }

    // 새 무기 생성 및 부착
    UStaticMeshComponent* WeaponMeshComponent = NewObject<UStaticMeshComponent>(Character);
    WeaponMeshComponent->SetStaticMesh(ItemData->EquipmentMesh);
    WeaponMeshComponent->RegisterComponent();
    WeaponMeshComponent->AttachToComponent(
        Character->GetMesh(),
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        ItemData->AttachSocketName
    );

    if (GetOwner()->HasAuthority())
    {
        EquippedWeapon = ItemData;
    }

    UE_LOG(LogHG, Log, TEXT("Equipped weapon: %s to socket %s"), *ItemData->ItemName.ToString(), *ItemData->AttachSocketName.ToString());
}

void UACShopComponent::UnequipWeapon()
{
    AACCharacter* Character = Cast<AACCharacter>(GetOwner());
    if (Character == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("UnequipWeapon: Owner is not AACCharacter"));
        return;
    }

    if (EquippedWeapon == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("UnequipWeapon: No weapon equipped"));
        return;
    }

    TArray<UStaticMeshComponent*> StaticMeshComponents;
    Character->GetComponents<UStaticMeshComponent>(StaticMeshComponents);

    for (UStaticMeshComponent* MeshComp : StaticMeshComponents)
    {
        if (MeshComp->GetAttachSocketName() == EquippedWeapon->AttachSocketName)
        {
            MeshComp->DestroyComponent();
            UE_LOG(LogHG, Log, TEXT("Unequipped weapon: %s from socket %s"), *EquippedWeapon->ItemName.ToString(), *EquippedWeapon->AttachSocketName.ToString());
            break;
        }
    }

    if (GetOwner()->HasAuthority())
    {
        EquippedWeapon = nullptr;
    }
}

void UACShopComponent::MulticastUnequipWeapon_Implementation()
{
    UnequipWeapon();
}

void UACShopComponent::ServerToggleWeaponEquip_Implementation(UACItemData* ItemData)
{
    if (ItemData == nullptr) return;

    // 아이템 이름으로 비교
    bool bIsSameWeapon = (EquippedWeapon != nullptr && EquippedWeapon->ItemName.EqualTo(ItemData->ItemName));

    if (bIsSameWeapon)
    {
        MulticastUnequipWeapon();
    }
    else
    {
        MulticastEquipItem(ItemData);
    }
}

void UACShopComponent::MulticastEquipItem_Implementation(UACItemData* ItemData)
{
    if (ItemData == nullptr) return;

    EquipItem(ItemData);
}

void UACShopComponent::ServerPurchaseAndEquipItem_Implementation(UACItemData* ItemData)
{
    if (ItemData == nullptr) return;

    if (PurchaseItem(ItemData))
    {
        MulticastEquipItem(ItemData);
    }
    else
    {
        UE_LOG(LogHG, Warning, TEXT("Server: Failed to purchase item %s"), *ItemData->ItemName.ToString());
    }
}

