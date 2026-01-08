// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTV/ACCCTVArea.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Component/ACInteractableComponent.h"
#include "Character/ACCharacter.h"
#include "UI/CCTV/ACCCTVWidget.h"
#include "UI/Interaction/ACInteractionInfoWidget.h"
#include "Interaction/ACInteractionData.h"
#include "Blueprint/UserWidget.h"
#include "AnimalCrime.h"
#include "Game/ACMainPlayerController.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/SceneCapture2D.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AACCCTVArea::AACCCTVArea()
{
	// RootComponent 생성
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	InteractBoxComponent = CreateDefaultSubobject<UACInteractableComponent>(TEXT("InteractBoxComponent"));
	InteractBoxComponent->SetupAttachment(RootComponent);
	InteractBoxComponent->SetMargin(FVector(200.0f, 200.0f, 200.0f));

	// 상호작용 위젯 컴포넌트
	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidgetComponent->SetupAttachment(RootComponent);
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidgetComponent->SetDrawSize(FVector2D(300.0f, 100.0f));
	InteractionWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f)); // 오브젝트 위
	InteractionWidgetComponent->SetVisibility(false); // 기본 숨김

    // 상호작용 위젯 클래스 설정
    static ConstructorHelpers::FClassFinder<UACInteractionInfoWidget> InteractionWidgetRef(
        TEXT("/Game/Project/UI/Interaction/WBP_InteractionInfo.WBP_InteractionInfo_C"));
    if (InteractionWidgetRef.Succeeded())
    {
        InteractionInfoWidgetClass = InteractionWidgetRef.Class;
    }
}

// Called when the game starts or when spawned
void AACCCTVArea::BeginPlay()
{
	Super::BeginPlay();	

    // Scene Capture 컴포넌트들 자동 수집
    if (SceneCaptureComponents.Num() == 0)
    {
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASceneCapture2D::StaticClass(), FoundActors);

        for (AActor* Actor : FoundActors)
        {
            ASceneCapture2D* SceneCapture = Cast<ASceneCapture2D>(Actor);
            if (SceneCapture && SceneCapture->GetCaptureComponent2D())
            {
                SceneCaptureComponents.Add(SceneCapture->GetCaptureComponent2D());
            }
        }
    }

    // 개별 카운터 배열 초기화
    ActiveViewerCountPerCapture.SetNum(SceneCaptureComponents.Num());
    for (int32 i = 0; i < ActiveViewerCountPerCapture.Num(); i++)
    {
        ActiveViewerCountPerCapture[i] = 0;
    }

    // 콜리전 종료 이벤트 바인딩
    if (InteractBoxComponent)
    {
        InteractBoxComponent->OnComponentEndOverlap.AddDynamic(
            this, &AACCCTVArea::OnInteractBoxOverlapEnd);
    }
}

void AACCCTVArea::OnInteract(AACCharacter* ACPlayer, EInteractionKey InKey)
{
    if (ACPlayer == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("OnInteract: ACPlayer is null"));
        return;
    }

    if (CCTVWidgetClass == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("OnInteract: CCTVWidgetClass is not set!"));
        return;
    }

    // PlayerController를 통해 상점 토글
    AACMainPlayerController* PC = ACPlayer->GetController<AACMainPlayerController>();
    if (PC == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("OnInteract: PlayerController is not AACMainPlayerController"));
        return;
    }

    // Scene Capture 활성화
    SetSceneCaptureActive(true);

    // 서버에서 클라이언트에게 위젯 토글 명령 전송
    PC->ClientToggleCCTVWidget(CCTVWidgetClass);
}

EACInteractorType AACCCTVArea::GetInteractorType() const
{
	return EACInteractorType::CCTVArea;
}

void AACCCTVArea::OnInteractBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    AACCharacter* ACPlayer = Cast<AACCharacter>(OtherActor);
    if (ACPlayer == nullptr)
    {
        return;
    }

    // 플레이어 컨트롤러를 통해 CCTV UI 닫기
    AACMainPlayerController* PC = ACPlayer->GetController<AACMainPlayerController>();
    if (PC != nullptr)
    {
        // Scene Capture 비활성화
        SetSceneCaptureActive(false);

        PC->CloseCCTV();
    }
}

void AACCCTVArea::SetSceneCaptureActive(bool bActive)
{
    // 참조 카운팅
    if (bActive == true)
    {
        ActiveViewerCount++;
    }
    else
    {
        ActiveViewerCount--;
        if (ActiveViewerCount < 0)
        {
            ActiveViewerCount = 0;
        }
    }

    UE_LOG(LogHG, Warning, TEXT("[CCTV] SetSceneCaptureActive: %d → ActiveViewerCount=%d"),
        bActive, ActiveViewerCount);

    // 각 Scene Capture를 개별적으로 제어
    for (int32 i = 0; i < SceneCaptureComponents.Num(); i++)
    {
        USceneCaptureComponent2D* SceneCapture = SceneCaptureComponents[i];
        if (SceneCapture != nullptr)
        {
            // 전체 카운터 OR 개별 카운터 중 하나라도 0보다 크면 활성화
            bool bShouldCapture = (ActiveViewerCount > 0) ||
                (ActiveViewerCountPerCapture.IsValidIndex(i) && ActiveViewerCountPerCapture[i] > 0);

            bool bWasCapturing = SceneCapture->bCaptureEveryFrame;
            SceneCapture->bCaptureEveryFrame = bShouldCapture;

            // 새로 활성화하거나, 이미 활성화된 경우에도 즉시 갱신
            if (bShouldCapture == true)
            {
                SceneCapture->CaptureScene();
            }
        }
    }
}

void AACCCTVArea::SetSceneCaptureActiveByIndex(int32 Index, bool bActive)
{
    if (SceneCaptureComponents.IsValidIndex(Index) == false)
    {
        UE_LOG(LogHG, Error, TEXT("[CCTV] Invalid Index: %d"), Index);
        return;
    }

    if (ActiveViewerCountPerCapture.IsValidIndex(Index) == false)
    {
        UE_LOG(LogHG, Error, TEXT("[CCTV] ActiveViewerCountPerCapture Invalid Index: %d"), Index);
        return;
    }

    // 개별 참조 카운팅
    if (bActive == true)
    {
        ActiveViewerCountPerCapture[Index]++;
    }
    else
    {
        ActiveViewerCountPerCapture[Index]--;
        if (ActiveViewerCountPerCapture[Index] < 0)
        {
            ActiveViewerCountPerCapture[Index] = 0;
        }
    }

    UE_LOG(LogHG, Warning, TEXT("[CCTV] SetSceneCaptureActiveByIndex(%d): %d → Count=%d"),
        Index, bActive, ActiveViewerCountPerCapture[Index]);

    // 해당 인덱스의 Scene Capture만 제어
    USceneCaptureComponent2D* SceneCapture = SceneCaptureComponents[Index];
    if (SceneCapture != nullptr)
    {
        bool bShouldCapture = (ActiveViewerCountPerCapture[Index] > 0);
        SceneCapture->bCaptureEveryFrame = bShouldCapture;

        if (bShouldCapture == true)
        {
            SceneCapture->CaptureScene();
        }
    }
}

UWidgetComponent* AACCCTVArea::GetInteractionWidget() const
{
	return InteractionWidgetComponent;
}

void AACCCTVArea::ShowInteractionHints(const TArray<UACInteractionData*>& Interactions)
{
	if (!InteractionWidgetComponent) return;

	// 위젯 생성 (최초 1회)
	if (!InteractionWidgetComponent->GetWidget() && InteractionInfoWidgetClass)
	{
		InteractionWidgetComponent->SetWidgetClass(InteractionInfoWidgetClass);
	}

	UACInteractionInfoWidget* HintWidget = Cast<UACInteractionInfoWidget>(
		InteractionWidgetComponent->GetWidget());
	if (!HintWidget) return;

	HintWidget->UpdateInteractions(Interactions);
	HintWidget->ShowWidget();
	InteractionWidgetComponent->SetVisibility(true);
}

void AACCCTVArea::HideInteractionHints()
{
	if (!InteractionWidgetComponent) return;

	UACInteractionInfoWidget* HintWidget = Cast<UACInteractionInfoWidget>(
		InteractionWidgetComponent->GetWidget());
	if (HintWidget)
		HintWidget->HideWidget();

	InteractionWidgetComponent->SetVisibility(false);
}
