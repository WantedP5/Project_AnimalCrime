// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ACItemPreviewGenerator.generated.h"

/**

    @class   UACItemPreviewGenerator
    @brief   에디터 월드에서 찍었던 아이템의 이미지를 텍스쳐2D로 생성해주는 클래스
    @details 에디터 전용이어서 런타임에서는 사용 불가 

**/
UCLASS(Blueprintable)
class ANIMALCRIME_API UACItemPreviewGenerator : public UObject
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
 /**
     @brief  단일 아이템 데이터의 프리뷰 이미지 생성
     @param  ItemData       - 이미지를 생성할 아이템 데이터
     @param  PreviewCapture - 프리뷰 캡쳐용 액터
     @param  SavePath       - 이미지 저장 경로 (예: "/Game/Item/Previews/")
     @retval                - 생성된 Texture2D 또는 nullptr
 **/
	UFUNCTION(BlueprintCallable, Category = "Item Preview")
    static UTexture2D* GeneratePreviewForItem(class UACItemData* ItemData, class AACItemPreviewCapture* PreviewCapture, const FString& SavePath);

    /**
        @brief  콘텐트 브라우저에서 모든 ItemData를 찾아서 프리뷰 이미지 생성
        @param  SearchPath     - 검색할 경로 (예: "/Game/Item/")
        @param  PreviewCapture - 프리뷰 캡쳐용 액터
        @param  SavePath       - 이미지 저장 경로 (예: "/Game/Item/Previews/")
        @retval                - 생성된 이미지 개수
    **/
    UFUNCTION(BlueprintCallable, Category = "Item Preview")
    static int32 GenerateAllPreviews(const FString& SearchPath, class AACItemPreviewCapture* PreviewCapture, const FString& SavePath);

    /**
        @brief  RenderTarget을  Texture2D로 변환하여 저장
        @param  RenderTarget - 변환할 렌더타겟
        @param  PackageName  - 저장할 패키지 이름 (전체 경로)
        @retval              - 생성된 Textrue2D 또는 nullptr
    **/
    static UTexture2D* SaveRenderTargetAsTexture2D(class UTextureRenderTarget2D* RenderTarget, const FString& PackageName);
#endif // WITH_EDITOR
};
