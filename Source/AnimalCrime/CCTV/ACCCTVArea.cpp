// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTV/ACCCTVArea.h"
#include "Components/BoxComponent.h"
#include "Component/ACInteractableComponent.h"
#include "Character/ACCharacter.h"
#include "UI/CCTV/ACCCTVWidget.h"
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

bool AACCCTVArea::CanInteract(AACCharacter* ACPlayer)
{
	// 모든 플레이어가 상호작용 가능
	return true;

	// 특정 타입만 허용하려면:
	// return ACPlayer->GetCharacterType() == EACCharacterType::Mafia;
}

void AACCCTVArea::OnInteract(AACCharacter* ACPlayer)
{
    //ShowInteractDebug(ACPlayer, GetName());

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
