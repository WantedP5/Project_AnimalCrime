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
#include "AssetRegistry/AssetRegistryModule.h"

UACShopWidget::UACShopWidget(const FObjectInitializer& ObjectInitializer)
{

}

void UACShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

    // 버튼 이벤트 바인딩
    if (WeaponButton)
        WeaponButton->OnClicked.AddDynamic(this, &UACShopWidget::OnWeaponButtonClicked);

    if (HeadButton)
        HeadButton->OnClicked.AddDynamic(this, &UACShopWidget::OnHeadButtonClicked);

    if (FaceAccButton)
        FaceAccButton->OnClicked.AddDynamic(this, &UACShopWidget::OnFaceAccButtonClicked);

    if (TopButton)
        TopButton->OnClicked.AddDynamic(this, &UACShopWidget::OnTopButtonClicked);

    if (BottomButton)
        BottomButton->OnClicked.AddDynamic(this, &UACShopWidget::OnBottomButtonClicked);

    if (ShoesButton)
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
        UE_LOG(LogHG, Error, TEXT("ACShopWidget: SlotWidgetClass is not set! Please set it in Blueprint."));
        return;
    }

    // AssetRegistry 모듈 로드
    FAssetRegistryModule& AssetRegistryModule =
        FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

    // 모든 ItemData 에셋 검색
    TArray<FAssetData> AssetDataList;
    FARFilter Filter;
    Filter.ClassPaths.Add(UACItemData::StaticClass()->GetClassPathName());
    Filter.PackagePaths.Add(FName(*SearchPath));
    Filter.bRecursivePaths = true; // 하위 폴더도 검색

    AssetRegistry.GetAssets(Filter, AssetDataList);

    UE_LOG(LogHG, Warning, TEXT("Found %d ItemData assets in %s"), AssetDataList.Num(), *SearchPath);

    // 3. 에셋 로드 및 타입별 분류
    AllItemData.Empty();
    WeaponItems.Empty();
    HeadItems.Empty();
    FaceAccItems.Empty();
    TopItems.Empty();
    BottomItems.Empty();
    ShoesItems.Empty();

    for (const FAssetData& AssetData : AssetDataList)
    {
        UACItemData* ItemData = Cast<UACItemData>(AssetData.GetAsset());
        if (ItemData != nullptr)
        {
            AllItemData.Add(ItemData);

            // 타입별 분류
            if (ItemData->ItemType == EItemType::Weapon)
            {
                WeaponItems.Add(ItemData);
            }
            else if (ItemData->ItemType == EItemType::Clothing)
            {
                // 의류는 슬롯별로 세부 분류
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
    }
   
    // 초기 카테고리 슬롯 생성
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
