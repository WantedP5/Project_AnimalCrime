
#include "Shop/ACShopArea.h"
#include "Components/BoxComponent.h"
#include "Component/ACInteractableComponent.h"
#include "Character/ACCharacter.h"
#include "UI/ACShopWidget.h"
#include "Blueprint/UserWidget.h"
#include "AnimalCrime.h"

// Sets default values
AACShopArea::AACShopArea()
{
	// RootComponent 생성
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;


	InteractBoxComponent = CreateDefaultSubobject<UACInteractableComponent>(TEXT("InteractBoxComponent"));
	InteractBoxComponent->SetupAttachment(RootComponent);

    InteractBoxComponent->SetMargin(FVector(200.0f, 200.0f, 200.0f));
}

// Called when the game starts or when spawned
void AACShopArea::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AACShopArea::CanInteract(AACCharacter* ACPlayer)
{
	// 모든 플레이어가 상호작용 가능
	return true;

	// 특정 타입만 허용하려면:
	// return ACPlayer->GetCharacterType() == EACCharacterType::Mafia;
}

void AACShopArea::OnInteract(AACCharacter* ACPlayer)
{
    ShowInteractDebug(ACPlayer);

    if (ACPlayer == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("OnInteract: ACPlayer is null"));
        return;
    }

    if (ShopWidgetClass == nullptr)
    {
        UE_LOG(LogHG, Error, TEXT("OnInteract: ShopWidgetClass is not set!"));
        return;
    }

    if (ACPlayer->IsLocallyControlled())
    {
        UACShopWidget** FoundWidget = ActiveShopWidgets.Find(ACPlayer);
        APlayerController* PC = ACPlayer->GetController<APlayerController>();

        if (PC == nullptr)
        {
            return;
        }

        if (FoundWidget != nullptr && *FoundWidget != nullptr)
        {
            // ===== 상점 닫기 =====
            UE_LOG(LogHG, Log, TEXT("Closing Shop UI for %s"), *ACPlayer->GetName());

            UACShopWidget* ShopWidget = *FoundWidget;
            ShopWidget->RemoveFromParent();

            // ⭐ 게임 입력 복구
            PC->SetShowMouseCursor(false);
            PC->SetInputMode(FInputModeGameOnly());
            PC->SetIgnoreMoveInput(false);    // 움직임 입력 허용
            PC->SetIgnoreLookInput(false);    // 시야 입력 허용

            ActiveShopWidgets.Remove(ACPlayer);
        }
        else
        {
            // ===== 상점 열기 =====
            UE_LOG(LogHG, Log, TEXT("Opening Shop UI for %s"), *ACPlayer->GetName());

            UACShopWidget* ShopWidget = CreateWidget<UACShopWidget>(
                ACPlayer->GetWorld(),
                ShopWidgetClass
            );

            if (ShopWidget != nullptr)
            {
                ShopWidget->AddToViewport();

                // UI + 게임 입력 동시 허용 (E키로 닫을 수 있게)
                PC->SetShowMouseCursor(true);

                FInputModeGameAndUI InputMode;
                InputMode.SetWidgetToFocus(ShopWidget->TakeWidget());
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                PC->SetInputMode(InputMode);

                // 캐릭터 움직임 멈추기
                PC->SetIgnoreMoveInput(true);     // WASD 입력 차단
                PC->SetIgnoreLookInput(true);     // 마우스 시야 입력 차단

                ShopWidget->LoadAndCreateSlots(TEXT("/Game/Project/Item/"));
                ActiveShopWidgets.Add(ACPlayer, ShopWidget);
            }
        }
    }
}

FString AACShopArea::GetInteractableName() const
{
	return TEXT("Shop");
}
