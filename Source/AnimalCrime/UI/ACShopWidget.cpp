// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ACShopWidget.h"
#include "ACShopWidget.h"
//#include "Components/Border.h"
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

    if (ClothingButton)
        ClothingButton->OnClicked.AddDynamic(this, &UACShopWidget::OnClothingButtonClicked);

    // 보더 배열에 등록
    //Borders = { WeaponBorder, ClothingBorder};

    // 초기 상태: Weapon ScrollBox만 보여주기
    ShowBorder(EBorderType::Weapon);

    // 아이템 로드 및 슬롯 생성 
    LoadAndCreateSlots(TEXT("/Game/Project/Item/"));
}

void UACShopWidget::LoadAndCreateSlots(const FString& SearchPath)
{
    if (SlotWidgetClass == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("ACShopWidget: SlotWidgetClass is not set! Please set it in Blueprint."));
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

    UE_LOG(LogTemp, Warning, TEXT("Found %d ItemData assets in %s"), AssetDataList.Num(), *SearchPath);

    // 3. 에셋 로드 및 타입별 분류
    AllItemData.Empty();
    WeaponItems.Empty();
    ClothingItems.Empty();

    for (const FAssetData& AssetData : AssetDataList)
    {
        UACItemData* ItemData = Cast<UACItemData>(AssetData.GetAsset());
        if (ItemData != nullptr)
        {
            AllItemData.Add(ItemData);

            // 타입별 분류
            switch (ItemData->ItemType)
            {
            case EItemType::Weapon:
                WeaponItems.Add(ItemData);
                break;
            case EItemType::Clothing:
                ClothingItems.Add(ItemData);
                break;
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Classified - Weapons: %d, Clothing: %d"), WeaponItems.Num(),
        ClothingItems.Num());

    //  각 타입별로 슬롯 생성
    CreateSlotsForType(EBorderType::Weapon);
    CreateSlotsForType(EBorderType::Clothing);
}

void UACShopWidget::CreateSlotsForType(EBorderType Type)
{
    if (SlotWidgetClass == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("SlotWidgetClass is not set!"));
        return;
    }

    if (ContentGridPanel == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("ContentGridPanel is null"));
        return;
    }

    // 타입에 따라 아이템 선택
    TArray<UACItemData*> ItemsToCreate;
    switch (Type)
    {
    case EBorderType::Weapon:
        ItemsToCreate = WeaponItems;
        break;

    case EBorderType::Clothing:
        ItemsToCreate = ClothingItems;
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
            UE_LOG(LogTemp, Error, TEXT("Failed to create slot widget"));
            continue;
        }

        // 슬롯에 아이템 데이터 설정
        NewSlot->SetItemData(ItemData);

        // UniformGridPanel에 추가 (행, 열 계산)
        int32 Row = i / ColumnsPerRow;
        int32 Column = i % ColumnsPerRow;
        ContentGridPanel->AddChildToUniformGrid(NewSlot, Row, Column);
        ContentGridPanel->SetSlotPadding(FMargin(10.0f));

        UE_LOG(LogTemp, Log, TEXT("Created slot for: %s at [%d, %d]"), *ItemData->ItemName.ToString(), Row,
            Column);
    }
}

void UACShopWidget::OnWeaponButtonClicked()
{
    ShowBorder(EBorderType::Weapon);
}

void UACShopWidget::OnClothingButtonClicked()
{
    ShowBorder(EBorderType::Clothing);
}

void UACShopWidget::ShowBorder(EBorderType Type)
{
    CurrentBorderType = Type;

    // 선택된 타입에 맞는 슬롯 생성
    CreateSlotsForType(Type);
}
