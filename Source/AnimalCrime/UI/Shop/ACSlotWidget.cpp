// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ACSlotWidget.h"
#include "Item/ACItemData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "ACSlotWidget.h"
#include "Character/ACCharacter.h"
#include "Component/ACShopComponent.h"
#include "AnimalCrime.h"

void UACSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

    // 구매 버튼 이벤트 바인딩
    if (PurchaseButton != nullptr)
    {
        PurchaseButton->OnClicked.AddDynamic(this, &UACSlotWidget::OnPurchaseButtonClicked);
    }
}

void UACSlotWidget::SetItemData(UACItemData* InItemData)
{
    if (InItemData == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("SetItemData: ItemData is null"));
        return;
    }

    ItemData = InItemData;

    // 미리 저장된 이미지 사용
    if (ItemPreviewImage != nullptr)
    {
        if (InItemData->PreviewImage)
        {
            // PreviewImage가 있으면 표시
            ItemPreviewImage->SetBrushFromTexture(InItemData->PreviewImage);
        }
        else
        {
            // PreviewImage가 없으면 투명 처리
            ItemPreviewImage->SetColorAndOpacity(FLinearColor(0.2f, 0.2f, 0.2f, 0.0f));
            UE_LOG(LogHG, Warning, TEXT("PreviewImage is missing for item: %s"), *InItemData->ItemName.ToString());
        }
    }

    // 아이템 정보 UI 업데이트
    if (ItemNameText != nullptr)
    {
        ItemNameText->SetText(InItemData->ItemName);
    }

    if (ItemPriceText != nullptr)
    {
        FText PriceText = FText::Format(FText::FromString(TEXT("{0}G")), InItemData->Price);
        ItemPriceText->SetText(PriceText);
    }
}


void UACSlotWidget::OnPurchaseButtonClicked()
{
    if (ItemData == nullptr) return;

    APlayerController* PC = GetOwningPlayer();
    if (PC == nullptr) return;

    AACCharacter* Character = Cast<AACCharacter>(PC->GetPawn());
    if (Character == nullptr) return;

    UACShopComponent* ShopComponent = Character->FindComponentByClass<UACShopComponent>();
    if (ShopComponent == nullptr) return;

    // 이 함수가 내부적으로 Server RPC 호출
    ShopComponent->PurchaseAndEquipItem(ItemData);
}
