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
    for (USceneCaptureComponent2D* SceneCapture : SceneCaptureComponents)
    {
        if (SceneCapture)
        {
            SceneCapture->bCaptureEveryFrame = bActive;

            // 활성화 시 즉시 한 번 캡처하여 화면 갱신
            if (bActive)
            {
                SceneCapture->CaptureScene();
            }
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
