// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CCTV/ACCCTVWidget.h"
#include "ACCCTVSlotWidget.h"

void UACCCTVWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeCCTVSlots();
}

void UACCCTVWidget::InitializeCCTVSlots()
{
    // 슬롯 위젯들을 배열로 관리
    TArray<UACCCTVSlotWidget*> Slots = { CCTVSlot1, CCTVSlot2, CCTVSlot3, CCTVSlot4 };

    // 각 슬롯에 데이터 설정
    for (int32 i = 0; i < Slots.Num(); ++i)
    {
        if (Slots[i] == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("InitializeCCTVSlots: CCTVSlot%d is null"), i + 1);
            continue;
        }

        // 배열에 데이터가 있으면 사용, 없으면 기본값
        if (CCTVSlotDataArray.IsValidIndex(i) == true)
        {
            Slots[i]->SetCCTVData(CCTVSlotDataArray[i].Material, CCTVSlotDataArray[i].CCTVName);
        }
        else
        {
            // 기본값으로 설정
            UE_LOG(LogTemp, Warning, TEXT("InitializeCCTVSlots: CCTVSlotDataArray[%d] not set, using default"), i);
            Slots[i]->SetCCTVData(nullptr, FString::Printf(TEXT("CCTV %d"), i + 1));
        }
    }
}
