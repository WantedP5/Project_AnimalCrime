// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuickSlot/ACQuickSlotWidget.h"
#include "Item/ACItemData.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "AnimalCrime.h"
#include "Character/ACCharacter.h"
#include "Component/ACShopComponent.h"

void UACQuickSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

    // 초기 이미지 숨김 처리
    if (MainItemImg != nullptr)
    {
        MainItemImg->SetVisibility(ESlateVisibility::Hidden);
        MainSlotHighlight->SetBrushColor(FLinearColor(1, 1, 1, 0));
        
    }

    if (SubItemImg != nullptr)
    {
        SubItemImg->SetVisibility(ESlateVisibility::Hidden);
        SubSlotHighlight->SetBrushColor(FLinearColor(1, 1, 1, 0));
    }

    // ShopComponent 바인딩
    BindShopComponent();
}

void UACQuickSlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // ShopComponent 바인딩이 안 되어 있으면 시도
    if (bShopComponentBound == false)
    {
        BindShopComponent();
    }
}

void UACQuickSlotWidget::AddItem(UACItemData* ItemData, bool bIsMainSlot)
{
    if (ItemData == nullptr)
    {
        return;
    }

    if (bIsMainSlot)
    {
        MainSlotItem = ItemData;
        // 메인 슬롯 이미지 업데이트
        if (MainItemImg != nullptr && ItemData->PreviewImage != nullptr)
        {
            MainItemImg->SetBrushFromTexture(ItemData->PreviewImage);
            MainItemImg->SetVisibility(ESlateVisibility::Visible);
        }
    }
    else
    {
        SubSlotItem = ItemData;
        // 서브 슬롯 이미지 업데이트
        if (SubItemImg != nullptr && ItemData->PreviewImage != nullptr)
        {
            SubItemImg->SetBrushFromTexture(ItemData->PreviewImage);
            SubItemImg->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

bool UACQuickSlotWidget::TryAddItem(UACItemData* ItemData)
{
    if (ItemData == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("TryAddItem: ItemData is null"));
        return false;
    }

    // 메인 슬롯이 비어있으면 메인 슬롯에 추가
    if (IsMainSlotEmpty() == true)
    {
        AddItem(ItemData, true);
        UE_LOG(LogHG, Log, TEXT("Item added to Main Slot: %s"), *ItemData->ItemName.ToString());
        return true;
    }
    // 메인 슬롯은 차있고 서브 슬롯이 비어있으면 서브 슬롯에 추가
    else if (IsSubSlotEmpty() == true)
    {
        AddItem(ItemData, false);
        UE_LOG(LogHG, Log, TEXT("Item added to Sub Slot: %s"), *ItemData->ItemName.ToString());
        return true;
    }
    // 둘 다 차있으면 추가 실패
    else
    {
        UE_LOG(LogHG, Warning, TEXT("QuickSlot is full! Cannot add item: %s"), *ItemData->ItemName.ToString());
        return false;
    }
}

void UACQuickSlotWidget::ToggleSlotEquip(int32 SlotIndex)
{
    // 슬롯 인덱스에 따라 어떤 아이템을 사용할지 결정
    UACItemData* TargetItem = nullptr;

    switch (SlotIndex)
    {
    case 1:
        TargetItem = MainSlotItem;
        break;
    case 2:
        TargetItem = SubSlotItem;
        break;
    default:
        UE_LOG(LogHG, Warning, TEXT("ToggleSlotEquip: Invalid slot index %d"), SlotIndex);
        return;
    }

    // 슬롯에 아이템이 없으면 리턴
    if (TargetItem == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("ToggleSlotEquip: No item in slot %d"), SlotIndex);
        return;
    }

    // PlayerController 가져오기
    APlayerController* PC = GetOwningPlayer();
    if (PC == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("ToggleSlotEquip: PlayerController is null"));
        return;
    }

    // 캐릭터 가져오기
    AACCharacter* Character = Cast<AACCharacter>(PC->GetPawn());
    if (Character == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("ToggleSlotEquip: Character is null"));
        return;
    }

    // ShopComponent 가져오기
    UACShopComponent* ShopComponent = Character->FindComponentByClass<UACShopComponent>();
    if (ShopComponent == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("ToggleSlotEquip: ShopComponent is null"));
        return;
    }

    // 무기 장착/해제 토글 (RPC)
    ShopComponent->ToggleWeaponEquip(TargetItem);
    UE_LOG(LogHG, Log, TEXT("Toggling Slot %d weapon: %s"), SlotIndex, *TargetItem->ItemName.ToString());
}

void UACQuickSlotWidget::BindShopComponent()
{
    APlayerController* PC = GetOwningPlayer();
    if (PC == nullptr) return;

    AACCharacter* Character = Cast<AACCharacter>(PC->GetPawn());
    if (Character == nullptr)
    {
        // Pawn이 없으면 타이머로 재시도
        UE_LOG(LogHG, Warning, TEXT("QuickSlot: Pawn 없음, 0.1초 후 재시도"));

        FTimerHandle RetryTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            RetryTimerHandle,
            this,
            &UACQuickSlotWidget::BindShopComponent,
            0.1f,  // 0.1초 후
            false  // 한 번만
        );
        return;
    }

    UACShopComponent* ShopComponent = Character->FindComponentByClass<UACShopComponent>();
    if (ShopComponent == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("QuickSlot: ShopComponent 없음"));
        return;
    }

    // 델리게이트 바인딩
    ShopComponent->OnWeaponEquippedChanged.AddDynamic(this, &UACQuickSlotWidget::UpdateHighlight);

    // 초기 하이라이트 설정
    UpdateHighlight(ShopComponent->EquippedWeapon);

    bShopComponentBound = true;
    UE_LOG(LogHG, Log, TEXT("QuickSlot: ShopComponent 바인딩 성공"));
}

void UACQuickSlotWidget::UpdateHighlight(UACItemData* EquippedWeapon)
{
    UE_LOG(LogHG, Log, TEXT("[UpdateHighlight] Weapon: %s"),
        EquippedWeapon ? *EquippedWeapon->ItemName.ToString() : TEXT("None"));

    // 모든 하이라이트를 먼저 숨김
    if (MainSlotHighlight)
    {
        MainSlotHighlight->SetBrushColor(FLinearColor(1, 1, 1, 0));
    }
    if (SubSlotHighlight)
    {
        SubSlotHighlight->SetBrushColor(FLinearColor(1, 1, 1, 0));
    }

    // 장착된 무기가 없으면 끝
    if (EquippedWeapon == nullptr)
    {
        return;
    }

    // 현재 장착된 무기와 매칭되는 슬롯의 하이라이트만 보이기
    // 메인 슬롯 체크
    if (MainSlotItem != nullptr &&
        MainSlotItem->ItemName.EqualTo(EquippedWeapon->ItemName))
    {
        MainSlotHighlight->SetBrushColor(FLinearColor(0, 1, 0, 1)); // 초록색 불투명
        UE_LOG(LogHG, Log, TEXT("MainSlot 하이라이트 켜짐"));
    }
    // 서브 슬롯 체크
    else if (SubSlotItem != nullptr &&
        SubSlotItem->ItemName.EqualTo(EquippedWeapon->ItemName))
    {
        SubSlotHighlight->SetBrushColor(FLinearColor(0, 1, 0, 1)); // 초록색 불투명
        UE_LOG(LogHG, Log, TEXT("SubSlot 하이라이트 켜짐"));
    }
}
