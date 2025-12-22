// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CCTV/ACCCTVSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UACCCTVSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UACCCTVSlotWidget::SetCCTVData(UMaterialInterface* Material, const FString& CCTVName)
{
    // 머티리얼 설정
    if (CCTVAreaImg != nullptr && Material != nullptr)
    {
        CCTVAreaImg->SetBrushFromMaterial(Material);
    }
    else if (CCTVAreaImg == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("SetCCTVData: CCTVAreaImg is null"));
    }
    else if (Material == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("SetCCTVData: Material is null"));
    }

    // 텍스트 설정
    if (CCTVNum != nullptr)
    {
        CCTVNum->SetText(FText::FromString(CCTVName));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SetCCTVData: CCTVNum is null"));
    }
}
