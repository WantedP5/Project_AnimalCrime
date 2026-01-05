// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Phone/ACPhoneWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/CCTV/ACCCTVSlotWidget.h"
#include "UI/CCTV/ACCCTVWidget.h"

void UACPhoneWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 버튼 클릭 이벤트 바인딩
    if (CCTVButton1 != nullptr)
    {
        CCTVButton1->OnClicked.AddDynamic(this, &UACPhoneWidget::OnCCTVButton1Clicked);
    }
    if (CCTVButton2 != nullptr)
    {
        CCTVButton2->OnClicked.AddDynamic(this, &UACPhoneWidget::OnCCTVButton2Clicked);
    }
    if (CCTVButton3 != nullptr)
    {
        CCTVButton3->OnClicked.AddDynamic(this, &UACPhoneWidget::OnCCTVButton3Clicked);
    }
    if (CCTVButton4 != nullptr)
    {
        CCTVButton4->OnClicked.AddDynamic(this, &UACPhoneWidget::OnCCTVButton4Clicked);
    }

    // ===== 뒤로가기 버튼 바인딩 =====
    if (HomeButton)
    {
        HomeButton->OnClicked.AddDynamic(this, &UACPhoneWidget::OnHomeButtonClicked);
    }

    // CCTV 슬롯 초기화
    InitializeCCTVSlots();

    // 처음에는 홈 화면 보여주기
    ShowHomeScreen();
}

void UACPhoneWidget::InitializeCCTVSlots()
{
    // CCTVSlotDataArray에서 데이터 가져와서 슬롯에 설정
    if (CCTVSlotDataArray.Num() >= 1 && CCTVSlot1)
    {
        CCTVSlot1->SetCCTVData(CCTVSlotDataArray[0].Material, CCTVSlotDataArray[0].CCTVName);

        // 버튼 텍스트 설정
        if (CCTVText1 != nullptr)
        {
            CCTVText1->SetText(FText::FromString(CCTVSlotDataArray[0].CCTVName));
        }
    }
    if (CCTVSlotDataArray.Num() >= 2 && CCTVSlot2)
    {
        CCTVSlot2->SetCCTVData(CCTVSlotDataArray[1].Material, CCTVSlotDataArray[1].CCTVName);

        // 버튼 텍스트 설정
        if (CCTVText2 != nullptr)
        {
            CCTVText2->SetText(FText::FromString(CCTVSlotDataArray[1].CCTVName));
        }
    }
    if (CCTVSlotDataArray.Num() >= 3 && CCTVSlot3)
    {
        CCTVSlot3->SetCCTVData(CCTVSlotDataArray[2].Material, CCTVSlotDataArray[2].CCTVName);

        // 버튼 텍스트 설정
        if (CCTVText3 != nullptr)
        {
            CCTVText3->SetText(FText::FromString(CCTVSlotDataArray[2].CCTVName));
        }
    }
    if (CCTVSlotDataArray.Num() >= 4 && CCTVSlot4)
    {
        CCTVSlot4->SetCCTVData(CCTVSlotDataArray[3].Material, CCTVSlotDataArray[3].CCTVName);

        // 버튼 텍스트 설정
        if (CCTVText4 != nullptr)
        {
            CCTVText4->SetText(FText::FromString(CCTVSlotDataArray[3].CCTVName));
        }
    }
}

void UACPhoneWidget::ShowHomeScreen()
{
    if (MainSwitcher != nullptr)
    {
        MainSwitcher->SetActiveWidgetIndex(0); // 홈 화면
    }
}

void UACPhoneWidget::ShowCCTVScreen(int32 CCTVIndex)
{
    // 메인 화면을 CCTV 뷰어로 전환
    if (MainSwitcher != nullptr)
    {
        MainSwitcher->SetActiveWidgetIndex(1); // CCTV 뷰어 화면
    }

    // 해당 CCTV 슬롯 보여주기
    if (CCTVSwitcher != nullptr)
    {
        CCTVSwitcher->SetActiveWidgetIndex(CCTVIndex);
    }
}


void UACPhoneWidget::OnCCTVButton1Clicked()
{
    ShowCCTVScreen(0); // CCTVSlot1
}

void UACPhoneWidget::OnCCTVButton2Clicked()
{
    ShowCCTVScreen(1); // CCTVSlot2
}

void UACPhoneWidget::OnCCTVButton3Clicked()
{
    ShowCCTVScreen(2); // CCTVSlot3
}

void UACPhoneWidget::OnCCTVButton4Clicked()
{
    ShowCCTVScreen(3); // CCTVSlot4
}

void UACPhoneWidget::OnHomeButtonClicked()
{
    ShowHomeScreen(); // 홈 화면으로 돌아가기
}