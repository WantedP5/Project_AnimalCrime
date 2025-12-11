// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ACShopComponent.h"
#include "Character/ACCharacter.h"
#include "AnimalCrime.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UACShopComponent::UACShopComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...
     // 네트워크 복제 활성화
    SetIsReplicatedByDefault(true);
}

void UACShopComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UACShopComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // 복제할 변수들 등록
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

    //// 돈이 충분한지 확인
    //if (!CanAfford(ItemData->Price))
    //{
    //    UE_LOG(LogHG, Warning, TEXT("Not enough money. Need: %d, Have: %d"),
    //        ItemData->Price, Money);
    //    return false;
    //}

    //// 돈 차감
    //Money -= ItemData->Price;

    return true;
}

void UACShopComponent::EquipItem(UACItemData* ItemData)
{
    if (ItemData == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("EquipItem: ItemData is null"));
        return;
    }

    // 아이템 타입에 따라 착용
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
    if (ItemData == nullptr)
    {
        return;
    }

    //// 구매 시도
    //if (PurchaseItem(ItemData))
    //{
    //    // 구매 성공 시 바로 착용
    //    EquipItem(ItemData);
    //    return true;
    //}

    //return false;

       // 서버로 요청
    ServerPurchaseAndEquipItem(ItemData);
}

void UACShopComponent::EquipClothing(UACItemData* ItemData)
{
    // 캐릭터 가져오기
    AACCharacter* Character = Cast<AACCharacter>(GetOwner());
    if (Character == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("EquipClothing: Owner is not AACCharacter"));
        return;
    }

    // ClothingMesh가 있는지 확인
    if (ItemData->ClothingMesh == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("EquipClothing: ClothingMesh is null for %s"),
            *ItemData->ItemName.ToString());
        return;
    }

    // 슬롯에 따라 해당 메시 컴포넌트 찾기
    USkeletalMeshComponent* TargetMeshComponent = nullptr;

    switch (ItemData->ClothingSlot)
    {
   //case EClothingSlot::FullBody:
     //   TargetMeshComponent = Character->GetTopMesh();
       // break;

    case EClothingSlot::Head:
        TargetMeshComponent = Character->GetHeadMesh();
        if (GetOwner()->HasAuthority())
        {
            EquippedHead = ItemData;
        }
        break;

    case EClothingSlot::FaceAcc:
        TargetMeshComponent = Character->GetFaceAccMesh();
        if (GetOwner()->HasAuthority())
        {
            EquippedFaceAcc = ItemData;
        }
        break;

    case EClothingSlot::Top:
        TargetMeshComponent = Character->GetTopMesh();
        if (GetOwner()->HasAuthority())
        {
            EquippedTop = ItemData;
        }
        break;

    case EClothingSlot::Bottom:
        TargetMeshComponent = Character->GetBottomMesh();
        if (GetOwner()->HasAuthority())
        {
            EquippedBottom = ItemData;
        }
        break;

    case EClothingSlot::Shoes:
        TargetMeshComponent = Character->GetShoesMesh();
        if (GetOwner()->HasAuthority())
        {
            EquippedShoes = ItemData;
        }
        break;

    default:
        UE_LOG(LogHG, Warning, TEXT("Unsupported ClothingSlot"));
        return;
    }

    // 메시 컴포넌트가 유효한지 확인
    if (TargetMeshComponent == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("TargetMeshComponent is null for slot"));
        return;
    }

    // 메시 교체
    TargetMeshComponent->SetSkeletalMesh(ItemData->ClothingMesh);

    // Leader Pose Component 설정 (애니메이션 동기화)
    TargetMeshComponent->SetLeaderPoseComponent(Character->GetMesh());

    //// 착용 중인 아이템 업데이트
    ////EquippedClothing.Add(ItemData->ClothingSlot, ItemData);
    //if (GetOwner()->HasAuthority())
    //{
    //    EquippedClothing.Add(ItemData->ClothingSlot, ItemData);
    //}

    UE_LOG(LogHG, Log, TEXT("Equipped clothing: %s to slot %d"),
        *ItemData->ItemName.ToString(), (int32)ItemData->ClothingSlot);
}

void UACShopComponent::EquipWeapon(UACItemData* ItemData)
{
    // 캐릭터 가져오기
    AACCharacter* Character = Cast<AACCharacter>(GetOwner());
    if (Character == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("EquipWeapon: Owner is not AACCharacter"));
        return;
    }

    // EquipmentMesh가 있는지 확인
    if (ItemData->EquipmentMesh == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("EquipWeapon: EquipmentMesh is null"));
        return;
    }

    // 기존 무기 제거 (있다면)
    if (EquippedWeapon != nullptr)
    {
        // 기존에 부착된 무기 컴포넌트 찾아서 제거
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
        ItemData->AttachSocketName  // 예: "hand_rSocket"
    );

    // 착용 중인 무기 업데이트
    //EquippedWeapon = ItemData;
    if (GetOwner()->HasAuthority())
    {
        EquippedWeapon = ItemData;
    }

    UE_LOG(LogHG, Log, TEXT("Equipped weapon: %s to socket %s"),
        *ItemData->ItemName.ToString(), *ItemData->AttachSocketName.ToString());
}

void UACShopComponent::MulticastEquipItem_Implementation(UACItemData* ItemData)
{
    if (ItemData == nullptr)
    {
        return;
    }

    UE_LOG(LogHG, Log, TEXT("MulticastEquipItem: %s"), *ItemData->ItemName.ToString());

    // 모든 클라이언트에서 착용 처리
    EquipItem(ItemData);
}

void UACShopComponent::ServerPurchaseAndEquipItem_Implementation(UACItemData* ItemData)
{
    if (ItemData == nullptr)
    {
        return;
    }


    // 서버에서 구매 처리
    if (PurchaseItem(ItemData))
    {
        // 구매 성공 시 모든 클라이언트에 착용 알림
        MulticastEquipItem(ItemData);
    }
    else
    {
        UE_LOG(LogHG, Warning, TEXT("Server: Failed to purchase item %s"), *ItemData->ItemName.ToString());
    }
}

