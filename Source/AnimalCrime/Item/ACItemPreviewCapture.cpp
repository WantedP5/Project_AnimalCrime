// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ACItemPreviewCapture.h"
#include "ACItemPreviewCapture.h"
#include "Item/ACItemData.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TextureRenderTarget2D.h"

// Sets default values
AACItemPreviewCapture::AACItemPreviewCapture()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

     // 루트 컴포넌트 생성
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;

    // SceneCapture 컴포넌트 생성 (카메라 역할)
    SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
    SceneCaptureComponent->SetupAttachment(RootComponent);
    SceneCaptureComponent->ProjectionType = ECameraProjectionMode::Perspective;
    SceneCaptureComponent->bCaptureEveryFrame = false; // 필요할 때만 캡처 (성능)
    SceneCaptureComponent->bCaptureOnMovement = false;
    SceneCaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList; // 특정 메시만 렌더링

    // 카메라 위치 설정 (메시를 보는 각도)
    SceneCaptureComponent->SetRelativeLocation(FVector(-CameraDistance, 0, 0));
    SceneCaptureComponent->SetRelativeRotation(FRotator(0, 0, 0));
    SceneCaptureComponent->FOVAngle = CameraFOV;

    // 의류용 SkeletalMesh 컴포넌트
    ClothingMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ClothingMeshComponent"));
    ClothingMeshComponent->SetupAttachment(RootComponent);
    ClothingMeshComponent->SetRelativeLocation(FVector(0, 0, 0));
    ClothingMeshComponent->SetVisibility(false);

    // 무기용 StaticMesh 컴포넌트
    EquipmentMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquipmentMeshComponent"));
    EquipmentMeshComponent->SetupAttachment(RootComponent);
    EquipmentMeshComponent->SetRelativeLocation(FVector(0, 0, 0));
    EquipmentMeshComponent->SetVisibility(false);
}


void AACItemPreviewCapture::BeginPlay()
{
    Super::BeginPlay();

    InitializeRenderTarget();
}

void AACItemPreviewCapture::InitializeRenderTarget(bool bForceRecreate)
{
    UE_LOG(LogTemp, Warning, TEXT("ACItemPreviewCapture InitializeRenderTarget Called! ForceRecreate: %d"),
        bForceRecreate);

    // 강제 재생성이면 기존 RenderTarget 제거
    if (bForceRecreate && RenderTarget)
    {
        RenderTarget = nullptr;
    }

    // 렌더 타겟이 없으면 생성
    if (RenderTarget == nullptr)
    {
        RenderTarget = NewObject<UTextureRenderTarget2D>(this);

        RenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA16f;

        RenderTarget->InitAutoFormat(RenderTargetSize, RenderTargetSize);
        RenderTarget->ClearColor = FLinearColor::Transparent; // 투명 배경
        //RenderTarget->bForceLinearGamma = false;
        RenderTarget->UpdateResourceImmediate(true);
    }

    // SceneCapture에 렌더 타겟 할당
    if (SceneCaptureComponent != nullptr)
    {
        SceneCaptureComponent->TextureTarget = RenderTarget;
        //SceneCaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;

        // 렌더링 설정
        SceneCaptureComponent->ShowFlags.SetAtmosphere(false);
        SceneCaptureComponent->ShowFlags.SetFog(false);
        SceneCaptureComponent->ShowFlags.SetSkyLighting(false);
        SceneCaptureComponent->ShowFlags.SetPostProcessing(false);  
        SceneCaptureComponent->ShowFlags.SetTemporalAA(false);      

        // 알파채널 지원
        //SceneCaptureComponent->CaptureSource = ESceneCaptureSource::SCS_FinalColorHDR;
        //SceneCaptureComponent->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
        SceneCaptureComponent->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
        SceneCaptureComponent->bCaptureEveryFrame = false;
        SceneCaptureComponent->bCaptureOnMovement = false;

        // ShowOnlyList 설정 - 원래대로
        SceneCaptureComponent->ShowOnlyComponents.Empty();
        SceneCaptureComponent->ShowOnlyComponents.Add(ClothingMeshComponent);
        SceneCaptureComponent->ShowOnlyComponents.Add(EquipmentMeshComponent);
        
    }
}

void AACItemPreviewCapture::SetItemData(UACItemData* InItemData)
{
    if (InItemData == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("SetItemData: ItemData is null"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("SetItemData called for: %s"), *InItemData->ItemName.ToString());

    CurrentItemData = InItemData;

    // 아이템 타입에 따라 메시 설정
    switch (InItemData->ItemType)
    {
    case EItemType::Clothing:
    {
        if (InItemData->ClothingMesh)
        {
            UE_LOG(LogTemp, Warning, TEXT("Setting Clothing Mesh: %s"), *InItemData->ClothingMesh->GetName());
            ClothingMeshComponent->SetSkeletalMesh(InItemData->ClothingMesh);
            ClothingMeshComponent->SetVisibility(true);
            EquipmentMeshComponent->SetVisibility(false);

            // 메시를 원점에 배치하고 정면을 보도록 회전
            ClothingMeshComponent->SetRelativeLocation(FVector(0, 0, 0));
            ClothingMeshComponent->SetRelativeRotation(FRotator(0, 90, 0));

            UE_LOG(LogTemp, Log, TEXT("Set Clothing Mesh: %s"), *InItemData->ItemName.ToString());
        }
        break;
    }

    case EItemType::Weapon:
    {
        if (InItemData->EquipmentMesh)
        {
            UE_LOG(LogTemp, Warning, TEXT("Setting Equipment Mesh: %s"), *InItemData->EquipmentMesh->GetName());
            EquipmentMeshComponent->SetStaticMesh(InItemData->EquipmentMesh);
            EquipmentMeshComponent->SetVisibility(true);
            ClothingMeshComponent->SetVisibility(false);

            // 메시를 원점에 배치하고 정면을 보도록 회전
            EquipmentMeshComponent->SetRelativeLocation(FVector(0, 0, 0));
            EquipmentMeshComponent->SetRelativeRotation(FRotator(0, 90, 0));

            UE_LOG(LogTemp, Log, TEXT("Set Equipment Mesh: %s"), *InItemData->ItemName.ToString());
        }
        break;
    }
    }

    // 카메라 위치 자동 조정
    AutoAdjustCamera();

    // ShowOnlyList 업데이트 및 캡처
    if (SceneCaptureComponent != nullptr)
    {
        SceneCaptureComponent->ShowOnlyComponents.Empty();
        SceneCaptureComponent->ShowOnlyComponents.Add(ClothingMeshComponent);
        SceneCaptureComponent->ShowOnlyComponents.Add(EquipmentMeshComponent);

        UE_LOG(LogTemp, Warning, TEXT("Calling CaptureScene..."));
        SceneCaptureComponent->CaptureScene();
        UE_LOG(LogTemp, Warning, TEXT("CaptureScene Complete!"));
    }
}

void AACItemPreviewCapture::AutoAdjustCamera()
{
    FBoxSphereBounds MeshBounds;
    bool bHasBounds = false;

    // 현재 보이는 메시의 바운드 가져오기
    if (ClothingMeshComponent && ClothingMeshComponent->IsVisible())
    {
        MeshBounds = ClothingMeshComponent->Bounds;
        bHasBounds = true;
    }
    else if (EquipmentMeshComponent && EquipmentMeshComponent->IsVisible())
    {
        MeshBounds = EquipmentMeshComponent->Bounds;
        bHasBounds = true;
    }

    if (!bHasBounds)
    {
        UE_LOG(LogTemp, Warning, TEXT("AutoAdjustCamera: No visible mesh"));
        return;
    }

    // 메시의 크기 계산
    float MeshRadius = MeshBounds.SphereRadius;
    FVector MeshCenter = MeshBounds.Origin;

    // FOV를 고려한 카메라 거리 계산
    float HalfFOVRadians = FMath::DegreesToRadians(CameraFOV * 0.5f);
    float DesiredDistance = MeshRadius / FMath::Tan(HalfFOVRadians);

    //// 여유 공간 추가 (1.5배)
    //DesiredDistance *= 1.0f;

    // 카메라 위치 조정 (X축 음수 방향으로 이동)
    FVector CameraLocation = MeshCenter + FVector(-DesiredDistance, 0, 0);
    SceneCaptureComponent->SetWorldLocation(CameraLocation);

    // 카메라가 메시 중심을 바라보도록 회전
    FRotator LookAtRotation = (MeshCenter - CameraLocation).Rotation();
    SceneCaptureComponent->SetWorldRotation(LookAtRotation);

    UE_LOG(LogTemp, Warning, TEXT("Camera adjusted - Distance: %.2f, MeshRadius: %.2f, Center: %s"),
        DesiredDistance, MeshRadius, *MeshCenter.ToString());
}
