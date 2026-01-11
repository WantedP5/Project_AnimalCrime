// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/ACShopComponent.h"
#include "Character/ACCharacter.h"
#include "AnimalCrime.h"
#include "Net/UnrealNetwork.h"
#include "UI/QuickSlot/ACQuickSlotWidget.h"
#include "UI/ACHUDWidget.h"
#include "Game/ACMainPlayerController.h"
#include "ACMoneyComponent.h"
#include "Objects/MoneyData.h"
#include "Game/ACPlayerState.h"
#include "UI/BoundItem/ACBoundItemWidget.h"  
#include "Game/ACMainPlayerController.h"   
#include "Character/ACMafiaCharacter.h"
#include "Engine/AssetManager.h"

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

UACItemData* LoadItemDataSync(const FPrimaryAssetId& AssetId)
{
    if (!AssetId.IsValid())
    {
        return nullptr;
    }

    UAssetManager& AssetManager = UAssetManager::Get();
    FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(AssetId);

    if (AssetPath.IsValid())
    {
        return Cast<UACItemData>(AssetPath.TryLoad());
    }

    return nullptr;
}

bool UACShopComponent::PurchaseItem(UACItemData* ItemData)
{
    if (ItemData == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("PurchaseItem: ItemData is null"));
        return false;
    }

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (Character == nullptr)
    {
        return false;
    }

    UACMoneyComponent* MoneyComponent = Character->FindComponentByClass<UACMoneyComponent>();

    if (MoneyComponent == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("MoneyComponent not found on Owner"));
        return false;
    }

    return MoneyComponent->SpendMoney(ItemData->Price);
}

void UACShopComponent::PurchaseSpecialItem(UACItemData* ItemData)
{
    if (ItemData == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("PurchaseSpecialItem: ItemData is null"));
        return;
    }


    // ===== 추가: 클라이언트에서 미리 체크 =====
    FString ItemNameStr = ItemData->ItemName.ToString();
    if (ItemNameStr.Contains(TEXT("무전기")) || ItemNameStr.Contains(TEXT("WalkyTalky")))
    {
        AActor* Owner = GetOwner();
        if (AACCharacter* Character = Cast<AACCharacter>(Owner))
        {
            AACMafiaCharacter* MafiaChar = Cast<AACMafiaCharacter>(Character);
            if (MafiaChar != nullptr && MafiaChar->HasWalkyTalky())
            {
                UE_LOG(LogHG, Warning, TEXT("무전기는 이미 소지하고 있습니다!"));
                return;  // 서버 RPC 호출 안함!
            }
        }
    }

    ServerPurchaseSpecialItem(ItemData->GetPrimaryAssetId());  // 변경
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

    ServerPurchaseAndEquipItem(ItemData->GetPrimaryAssetId());  // 변경
}

void UACShopComponent::PurchaseAndAddToQuickSlot(UACItemData* ItemData)
{
    if (ItemData == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("PurchaseAndAddToQuickSlot: ItemData is null"));
        return;
    }

    // 퀵슬롯 위젯 가져오기
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

    UACQuickSlotWidget* QuickSlot = PC->ACHUDWidget->WBP_QuickSlot;

    // 1. 퀵슬롯이 가득 찼는지 체크
    if (QuickSlot->IsFull() == true)
    {
        UE_LOG(LogHG, Warning, TEXT("QuickSlot is full! Cannot purchase."));
        return;
    }

    // 2. 같은 아이템이 이미 있는지 체크
    if (QuickSlot->HasSameItem(ItemData) == true)
    {
        UE_LOG(LogHG, Warning, TEXT("Same item already exists in QuickSlot! Cannot purchase."));
        return;
    }

    // 서버 RPC 호출
    ServerPurchaseAndAddToQuickSlot(ItemData->GetPrimaryAssetId());  // 변경
}

void UACShopComponent::PurchaseAmmo(UACItemData* ItemData)
{
    if (ItemData == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("PurchaseAmmo: ItemData is null"));
        return;
    }

    if (ItemData->ItemType != EItemType::Ammo)
    {
        UE_LOG(LogHG, Warning, TEXT("PurchaseAmmo: ItemData is not Ammo type"));
        return;
    }

    // 서버 RPC 호출
    ServerPurchaseAmmo(ItemData->GetPrimaryAssetId());
}

void UACShopComponent::ToggleWeaponEquip(UACItemData* ItemData)
{
    if (ItemData == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("ToggleWeaponEquip: ItemData is null"));
        return;
    }

    ServerToggleWeaponEquip(ItemData->GetPrimaryAssetId());  // 변경
}

void UACShopComponent::OnRep_EquippedWeapon()
{
    // 델리게이트 브로드캐스트
    OnWeaponEquippedChanged.Broadcast(EquippedWeapon);
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
    WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponMeshComponent->RegisterComponent();
    WeaponMeshComponent->AttachToComponent(
        Character->GetMesh(),
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        ItemData->AttachSocketName
    );

    if (GetOwner()->HasAuthority())
    {
        EquippedWeapon = ItemData;

        // 서버에서 델리게이트 브로드캐스트
        OnWeaponEquippedChanged.Broadcast(EquippedWeapon);
    }
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
            break;
        }
    }

    if (GetOwner()->HasAuthority())
    {
        EquippedWeapon = nullptr;

        // 서버에서 델리게이트 브로드캐스트
        OnWeaponEquippedChanged.Broadcast(nullptr);
    }
}

void UACShopComponent::ClientAddToQuickSlot_Implementation(FPrimaryAssetId ItemAssetId)
{
    UACItemData* ItemData = LoadItemDataSync(ItemAssetId);
    if (ItemData == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("ClientAddToQuickSlot: Failed to load ItemData"));
        return;
    }

    AACCharacter* Character = Cast<AACCharacter>(GetOwner());
    if (Character == nullptr) return;

    AACMainPlayerController* PC = Character->GetController<AACMainPlayerController>();
    if (PC == nullptr || PC->ACHUDWidget == nullptr || PC->ACHUDWidget->WBP_QuickSlot == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("ClientAddToQuickSlot: QuickSlot widget is null"));
        return;
    }

    if (PC->ACHUDWidget->WBP_QuickSlot->IsFull())
    {
        return;
    }

    if (PC->ACHUDWidget->WBP_QuickSlot->TryAddItem(ItemData))
    {
        //UE_LOG(LogHG, Log, TEXT("Item added to quickslot: %s"), *ItemData->ItemName.ToString());
    }
}

void UACShopComponent::ServerPurchaseAndAddToQuickSlot_Implementation(FPrimaryAssetId ItemAssetId)
{
    UACItemData* ItemData = LoadItemDataSync(ItemAssetId);
    if (ItemData == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("ServerPurchaseAndAddToQuickSlot: Failed to load ItemData"));
        return;
    }

    // 서버에서도 한번 더 체크
    AACCharacter* Character = Cast<AACCharacter>(GetOwner());
    if (Character == nullptr) return;

    AACMainPlayerController* PC = Character->GetController<AACMainPlayerController>();
    if (PC && PC->ACHUDWidget && PC->ACHUDWidget->WBP_QuickSlot)
    {
        UACQuickSlotWidget* QuickSlot = PC->ACHUDWidget->WBP_QuickSlot;

        if (QuickSlot->IsFull())
        {
            UE_LOG(LogHG, Warning, TEXT("Server: QuickSlot is full! Purchase blocked."));
            return;
        }

        if (QuickSlot->HasSameItem(ItemData))
        {
            UE_LOG(LogHG, Warning, TEXT("Server: Same item already exists! Purchase blocked."));
            return;
        }
    }

    if (PurchaseItem(ItemData) == false)
    {
        UE_LOG(LogHG, Warning, TEXT("Server: Failed to purchase item %s"), *ItemData->ItemName.ToString());
        return;
    }

    // 나중에 사라져야할 코드
    if (ItemData->ItemName.ToString() == TEXT("Pistol_001"))
    {
        UE_LOG(LogHY, Error, TEXT("Test Name:%s"), *GetOwner()->GetName());
        AACCharacter* CharacterPawn = Cast<AACCharacter>(GetOwner());
        if (CharacterPawn == nullptr)
        {
            UE_LOG(LogHY, Error, TEXT("None Test Name:%s"), *GetOwner()->GetName());
            return;
        }
        CharacterPawn->AddBullets(10000);
    }

    ClientAddToQuickSlot(ItemAssetId);  // AssetId 전달
}

void UACShopComponent::MulticastUnequipWeapon_Implementation()
{
    UnequipWeapon();
}

void UACShopComponent::ServerToggleWeaponEquip_Implementation(FPrimaryAssetId ItemAssetId)
{
    UACItemData* ItemData = LoadItemDataSync(ItemAssetId);
    if (ItemData == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("ServerToggleWeaponEquip: Failed to load ItemData"));
        return;
    }

    bool bIsSameWeapon = (EquippedWeapon != nullptr && EquippedWeapon->ItemName.EqualTo(ItemData->ItemName));

    if (bIsSameWeapon)
    {
        MulticastUnequipWeapon();
    }
    else
    {
        MulticastEquipItem(ItemAssetId);  // AssetId 전달
    }
}

void UACShopComponent::MulticastEquipItem_Implementation(FPrimaryAssetId ItemAssetId)
{
    UACItemData* ItemData = LoadItemDataSync(ItemAssetId);
    if (ItemData == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("MulticastEquipItem: Failed to load ItemData"));
        return;
    }

    EquipItem(ItemData);
}

void UACShopComponent::ServerPurchaseAndEquipItem_Implementation(FPrimaryAssetId ItemAssetId)
{
    UACItemData* ItemData = LoadItemDataSync(ItemAssetId);
    if (ItemData == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("ServerPurchaseAndEquipItem: Failed to load ItemData"));
        return;
    }

    if (PurchaseItem(ItemData))
    {
        MulticastEquipItem(ItemAssetId);  // 여기도 AssetId 전달
    }
    else
    {
        UE_LOG(LogHG, Warning, TEXT("Server: Failed to purchase item %s"), *ItemData->ItemName.ToString());
    }
}

void UACShopComponent::ServerPurchaseSpecialItem_Implementation(FPrimaryAssetId ItemAssetId)
{
	UACItemData* ItemData = LoadItemDataSync(ItemAssetId);
	if (ItemData == nullptr)
	{
		UE_LOG(LogHG, Error, TEXT("ServerPurchaseSpecialItem: Failed to load ItemData"));
		return;
	}

	AActor* Owner = GetOwner();
	if (Owner == nullptr)
	{
		UE_LOG(LogHG, Error, TEXT("ServerPurchaseSpecialItem: Owner is null"));
		return;
	}
	AACCharacter* Character = Cast<AACCharacter>(Owner);
	if (Character == nullptr)
	{
		UE_LOG(LogHG, Error, TEXT("ServerPurchaseSpecialItem: Owner is not AACCharacter"));
		return;
	}

	// 구매할 아이템이 무전기면 특수 처리
	FString ItemNameStr = ItemData->ItemName.ToString();
	if (ItemNameStr.Contains(TEXT("무전기")) || ItemNameStr.Contains(TEXT("WalkyTalky")))
	{
		if (Character->GetCharacterType() == EACCharacterType::Police)
		{
			UE_LOG(LogHG, Warning, TEXT("서버: 경찰은 무전기를 구매할 수 없습니다!"));
			return;
		}

		AACMafiaCharacter* MafiaChar = Cast<AACMafiaCharacter>(Character);
		if (MafiaChar != nullptr && MafiaChar->HasWalkyTalky())
		{
			UE_LOG(LogHG, Warning, TEXT("서버: 무전기는 이미 소지하고 있습니다!"));
			return;
		}

		// 무전기 구매 성공 후 음성 그룹 마피아로 설정
		Character->SetVoiceGroup(EVoiceGroup::Mafia);
	}

	if (PurchaseItem(ItemData) == false)
	{
		UE_LOG(LogHG, Warning, TEXT("특수 아이템 구매 실패: %s"), *ItemData->ItemName.ToString());
		return;
	}

	ClientNotifySpecialItemPurchased(ItemAssetId);  // AssetId 전달
}

void UACShopComponent::ServerPurchaseAmmo_Implementation(FPrimaryAssetId ItemAssetId)
{
    UACItemData* ItemData = LoadItemDataSync(ItemAssetId);
    if (ItemData == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("ServerPurchaseAmmo: Failed to load ItemData"));
        return;
    }

    if (ItemData->ItemType != EItemType::Ammo)
    {
        UE_LOG(LogHG, Warning, TEXT("ServerPurchaseAmmo: ItemData is not Ammo type"));
        return;
    }

    // 서버에서 구매 처리
    if (PurchaseItem(ItemData) == false)
    {
        UE_LOG(LogHG, Warning, TEXT("ServerPurchaseAmmo: Failed to purchase"));
        return;
    }

    // 서버에서 총알 추가 (자동으로 클라이언트에 복제됨)
    AACCharacter* Character = Cast<AACCharacter>(GetOwner());
    if (Character == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("ServerPurchaseAmmo: Owner is not AACCharacter"));
        return;
    }

    Character->AddBullets(ItemData->AmmoCount);
    UE_LOG(LogHG, Log, TEXT("서버: 탄약 구매 성공! +%d발"), ItemData->AmmoCount);
}

void UACShopComponent::ClientNotifySpecialItemPurchased_Implementation(FPrimaryAssetId ItemAssetId)
{
    UACItemData* ItemData = LoadItemDataSync(ItemAssetId);
    if (ItemData == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("ClientNotifySpecialItemPurchased: Failed to load ItemData"));
        return;
    }

    AActor* Owner = GetOwner();
    if (AACCharacter* Character = Cast<AACCharacter>(Owner))
    {
        FString ItemNameStr = ItemData->ItemName.ToString();

        if (ItemNameStr.Contains(TEXT("무전기")) || ItemNameStr.Contains(TEXT("WalkyTalky")))
        {
            AACMafiaCharacter* MafiaChar = Cast<AACMafiaCharacter>(Character);
            if (MafiaChar == nullptr)
            {
                UE_LOG(LogHG, Warning, TEXT("마피아만 무전기를 구매할 수 있습니다!"));
                return;
            }

            if (MafiaChar->HasWalkyTalky())
            {
                UE_LOG(LogHG, Error, TEXT("클라이언트: 무전기 중복 - 이 메시지가 나오면 안됨!"));
                return;
            }

            MafiaChar->SetWalkyTalky(true);
            UE_LOG(LogHG, Log, TEXT("무전기 획득!"));
        }
    }
}