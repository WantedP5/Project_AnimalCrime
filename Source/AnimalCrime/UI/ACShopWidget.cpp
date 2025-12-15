// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ACShopWidget.h"
#include "ACShopWidget.h"
#include "AnimalCrime.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include "ACSlotWidget.h"
#include "Item/ACItemData.h"
#include "Game/ACAssetManager.h"
#include "Components/Image.h"

UACShopWidget::UACShopWidget(const FObjectInitializer& ObjectInitializer)
{

}

void UACShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

    // 버튼 이벤트 바인딩
    if (WeaponButton != nullptr)
        WeaponButton->OnClicked.AddDynamic(this, &UACShopWidget::OnWeaponButtonClicked);

    if (HeadButton != nullptr)
        HeadButton->OnClicked.AddDynamic(this, &UACShopWidget::OnHeadButtonClicked);

    if (FaceAccButton != nullptr)
        FaceAccButton->OnClicked.AddDynamic(this, &UACShopWidget::OnFaceAccButtonClicked);

    if (TopButton != nullptr)
        TopButton->OnClicked.AddDynamic(this, &UACShopWidget::OnTopButtonClicked);

    if (BottomButton != nullptr)
        BottomButton->OnClicked.AddDynamic(this, &UACShopWidget::OnBottomButtonClicked);

    if (ShoesButton != nullptr)
        ShoesButton->OnClicked.AddDynamic(this, &UACShopWidget::OnShoesButtonClicked);

    // 보더 배열에 등록
    //Borders = { WeaponBorder, ClothingBorder};

   // 초기 상태: Weapon 카테고리 보여주기
    ShowCategory(EShopCategory::Weapon);

    // 아이템 로드 및 슬롯 생성 
    LoadAndCreateSlots(TEXT("/Game/Project/Item/"));
}

void UACShopWidget::LoadAndCreateSlots(const FString& SearchPath)
{
    if (SlotWidgetClass == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("ACShopWidget: SlotWidgetClass is not set!"));
        return;
    }

    // ===== AssetManager에서 자동으로 가져오기 =====
    UE_LOG(LogHG, Warning, TEXT("Attempting to get AssetManager..."));

    if (GEngine == nullptr || GEngine->AssetManager == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("GEngine or AssetManager is null!"));
        return;
    }

    UACAssetManager* AssetManager = Cast<UACAssetManager>(GEngine->AssetManager);
    if (AssetManager == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("AssetManager is not UACAssetManager! Check DefaultEngine.ini AssetManagerClassName"));
        return;
    }

    UE_LOG(LogHG, Warning, TEXT("AssetManager obtained successfully"));
    TArray<UACItemData*> LoadedItems = AssetManager->GetAllItemData();

    // 배열 초기화
    AllItemData.Empty();
    WeaponItems.Empty();
    HeadItems.Empty();
    FaceAccItems.Empty();
    TopItems.Empty();
    BottomItems.Empty();
    ShoesItems.Empty();

    AllItemData = LoadedItems;

    UE_LOG(LogHG, Warning, TEXT("Loaded %d ItemData from AssetManager"), AllItemData.Num());

    if (AllItemData.Num() == 0)
    {
        UE_LOG(LogHG, Error, TEXT("No ItemData loaded from AssetManager!"));
        return;
    }

    // 카테고리별 분류
    for (UACItemData* ItemData : AllItemData)
    {
        if (ItemData == nullptr) continue;

        if (ItemData->ItemType == EItemType::Weapon)
        {
            WeaponItems.Add(ItemData);
        }
        else if (ItemData->ItemType == EItemType::Clothing)
        {
            switch (ItemData->ClothingSlot)
            {
            case EClothingSlot::Head:
                HeadItems.Add(ItemData);
                break;
            case EClothingSlot::FaceAcc:
                FaceAccItems.Add(ItemData);
                break;
            case EClothingSlot::Top:
                TopItems.Add(ItemData);
                break;
            case EClothingSlot::Bottom:
                BottomItems.Add(ItemData);
                break;
            case EClothingSlot::Shoes:
                ShoesItems.Add(ItemData);
                break;
            }
        }
    }

    UE_LOG(LogHG, Warning, TEXT("Categorized: Weapon=%d, Head=%d, FaceAcc=%d, Top=%d, Bottom=%d, Shoes=%d"),
        WeaponItems.Num(), HeadItems.Num(), FaceAccItems.Num(), TopItems.Num(), BottomItems.Num(), ShoesItems.Num());

    CreateSlotsForCategory(CurrentCategory);
}

void UACShopWidget::CreateSlotsForCategory(EShopCategory Category)
{
    if (SlotWidgetClass == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("SlotWidgetClass is not set!"));
        return;
    }

    if (ContentGridPanel == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("ContentGridPanel is null"));
        return;
    }

    // 카테고리에 따라 아이템 선택
    TArray<UACItemData*> ItemsToCreate;
    switch (Category)
    {
    case EShopCategory::Weapon:
        ItemsToCreate = WeaponItems;
        break;

    case EShopCategory::Head:
        ItemsToCreate = HeadItems;
        break;

    case EShopCategory::FaceAcc:
        ItemsToCreate = FaceAccItems;
        break;

    case EShopCategory::Top:
        ItemsToCreate = TopItems;
        break;

    case EShopCategory::Bottom:
        ItemsToCreate = BottomItems;
        break;

    case EShopCategory::Shoes:
        ItemsToCreate = ShoesItems;
        break;
    }

    // 기존 슬롯 제거
    ContentGridPanel->ClearChildren();

    // 그리드 설정 (한 줄에 몇 개씩 표시할지)
    const int32 ColumnsPerRow = 4;

    // 슬롯 생성 및 배치
    for (int32 i = 0; i < ItemsToCreate.Num(); ++i)
    {
        UACItemData* ItemData = ItemsToCreate[i];
        if (ItemData == nullptr)
        {
            continue;
        }

        // 슬롯 위젯 생성
        UACSlotWidget* NewSlot = CreateWidget<UACSlotWidget>(this, SlotWidgetClass);
        if (NewSlot == nullptr)
        {
            UE_LOG(LogHG, Error, TEXT("Failed to create slot widget"));
            continue;
        }

        // 슬롯에 아이템 데이터 설정
        NewSlot->SetItemData(ItemData);

        // UniformGridPanel에 추가 (행, 열 계산)
        int32 Row = i / ColumnsPerRow;
        int32 Column = i % ColumnsPerRow;
        ContentGridPanel->AddChildToUniformGrid(NewSlot, Row, Column);
        ContentGridPanel->SetSlotPadding(FMargin(10.0f));

        UE_LOG(LogHG, Log, TEXT("Created slot for: %s at [%d, %d]"), *ItemData->ItemName.ToString(), Row,
            Column);
    }
}

// 버튼 클릭 이벤트들
void UACShopWidget::OnWeaponButtonClicked()
{
    ShowCategory(EShopCategory::Weapon);
}

void UACShopWidget::OnHeadButtonClicked()
{
    ShowCategory(EShopCategory::Head);
}

void UACShopWidget::OnFaceAccButtonClicked()
{
    ShowCategory(EShopCategory::FaceAcc);
}

void UACShopWidget::OnTopButtonClicked()
{
    ShowCategory(EShopCategory::Top);
}

void UACShopWidget::OnBottomButtonClicked()
{
    ShowCategory(EShopCategory::Bottom);
}

void UACShopWidget::OnShoesButtonClicked()
{
    ShowCategory(EShopCategory::Shoes);
}

void UACShopWidget::ShowCategory(EShopCategory Category)
{
    CurrentCategory = Category;

    // 선택된 카테고리에 맞는 슬롯 생성
    CreateSlotsForCategory(Category);
}

