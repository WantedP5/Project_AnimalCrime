// Fill out your copyright notice in the Description page of Project Settings.


#include "Helper/ACItemPreviewGenerator.h"
#include "ACItemPreviewGenerator.h"
#include "Item/ACItemData.h"
#include "Item/ACItemPreviewCapture.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Texture2D.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#include "ImageUtils.h"
#include "Editor.h"
#include "Engine/World.h" 

UTexture2D* UACItemPreviewGenerator::GeneratePreviewForItem(UACItemData* ItemData, AACItemPreviewCapture* PreviewCapture, const FString& SavePath)
{
    if (!ItemData || !PreviewCapture)
    {
        UE_LOG(LogTemp, Error, TEXT("GeneratePreviewForItem: Invalid parameters"));
        return nullptr;
    }

    // ⭐ 에디터 월드 가져오기
    UWorld* EditorWorld = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
    if (!EditorWorld)
    {
        UE_LOG(LogTemp, Error, TEXT("GeneratePreviewForItem: No Editor World found"));
        return nullptr;
    }

    // 렌더타겟 초기화
    PreviewCapture->InitializeRenderTarget(true);

    // 아이템 설정
    PreviewCapture->SetItemData(ItemData);

    // ⭐⭐⭐ 핵심: 월드 틱을 수동으로 실행하여 렌더링 강제 ⭐⭐⭐
    EditorWorld->Tick(LEVELTICK_All, 0.1f); // 0.1초 틱
    FlushRenderingCommands();

    // 한 번 더 (확실하게)
    EditorWorld->Tick(LEVELTICK_All, 0.1f);
    FlushRenderingCommands();

    // RenderTarget 가져오기
    UTextureRenderTarget2D* RenderTarget = PreviewCapture->GetRenderTarget();
    if (!RenderTarget)
    {
        UE_LOG(LogTemp, Error, TEXT("GeneratePreviewForItem: RenderTarget is null"));
        return nullptr;
    }

    // 저장할 패키지 경로 생성
    FString AssetName = ItemData->GetName() + TEXT("_Preview");
    FString PackageName = SavePath + AssetName;

    // RenderTarget을 Texture2D로 저장
    UTexture2D* SavedTexture = SaveRenderTargetAsTexture2D(RenderTarget, PackageName);

    if (SavedTexture)
    {
        ItemData->PreviewImage = SavedTexture;
        ItemData->MarkPackageDirty();

        UE_LOG(LogTemp, Log, TEXT("Successfully generated preview for: %s"), *ItemData->ItemName.ToString());
    }

    return SavedTexture;
}

int32 UACItemPreviewGenerator::GenerateAllPreviews(const FString& SearchPath, AACItemPreviewCapture* PreviewCapture, const FString& SavePath)
{
    if (!PreviewCapture)
    {
        UE_LOG(LogTemp, Error, TEXT("GenerateAllPreviews: PreviewCapture is null"));
        return 0;
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
    Filter.bRecursivePaths = true;

    AssetRegistry.GetAssets(Filter, AssetDataList);

    UE_LOG(LogTemp, Warning, TEXT("Found %d ItemData assets in %s"), AssetDataList.Num(), *SearchPath);

    int32 GeneratedCount = 0;

    // 각 ItemData에 대해 프리뷰 생성
    for (const FAssetData& AssetData : AssetDataList)
    {
        UACItemData* ItemData = Cast<UACItemData>(AssetData.GetAsset());
        if (ItemData)
        {
            UTexture2D* PreviewTexture = GeneratePreviewForItem(ItemData, PreviewCapture, SavePath);
            if (PreviewTexture)
            {
                GeneratedCount++;
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Generated %d preview images"), GeneratedCount);
    return GeneratedCount;
}

UTexture2D* UACItemPreviewGenerator::SaveRenderTargetAsTexture2D(UTextureRenderTarget2D* RenderTarget, const FString& PackageName)
{
    if (!RenderTarget)
    {
        return nullptr;
    }

    // RenderTarget 데이터를 읽어오기
    FTextureRenderTargetResource* RTResource = RenderTarget->GameThread_GetRenderTargetResource();
    if (!RTResource)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get RenderTarget resource"));
        return nullptr;
    }

    // 이미지 데이터 읽기
    TArray<FColor> SurfaceData;
    if (!RTResource->ReadPixels(SurfaceData))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read pixels from RenderTarget"));
        return nullptr;
    }

    // 패키지 생성 또는 로드
    UPackage* Package = CreatePackage(*PackageName);
    Package->FullyLoad();

    // Texture2D 생성
    FString AssetName = FPackageName::GetLongPackageAssetName(PackageName);
    UTexture2D* NewTexture = NewObject<UTexture2D>(Package, FName(*AssetName), RF_Public | RF_Standalone);

    // Texture2D 설정
    int32 Width = RenderTarget->SizeX;
    int32 Height = RenderTarget->SizeY;

    NewTexture->Source.Init(Width, Height, 1, 1, TSF_BGRA8);

    // 픽셀 데이터 복사
    uint8* MipData = NewTexture->Source.LockMip(0);
    FMemory::Memcpy(MipData, SurfaceData.GetData(), Width * Height * sizeof(FColor));
    NewTexture->Source.UnlockMip(0);

    // Texture2D 설정 마무리
    NewTexture->SRGB = true;
    NewTexture->CompressionSettings = TC_Default;
    NewTexture->MipGenSettings = TMGS_NoMipmaps;
    // Alpha 채널 설정 - 배경 투명, 오브젝트 불투명
    NewTexture->AdjustMinAlpha = 1.0f;
    NewTexture->AdjustMaxAlpha = 0.0f;
    NewTexture->UpdateResource();

    // 패키지 저장
    Package->MarkPackageDirty();

    FSavePackageArgs SaveArgs;
    SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
    SaveArgs.SaveFlags = SAVE_NoError;

    FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName,
        FPackageName::GetAssetPackageExtension());

    if (UPackage::SavePackage(Package, NewTexture, *PackageFileName, SaveArgs))
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully saved texture: %s"), *PackageName);

        // Asset Registry에 알림
        FAssetRegistryModule::AssetCreated(NewTexture);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save texture: %s"), *PackageName);
        return nullptr;
    }

    return NewTexture;
}
