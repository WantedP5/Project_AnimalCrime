
#include "Shop/ACShopArea.h"
#include "Components/BoxComponent.h"
#include "Component/ACInteractableComponent.h"
#include "Character/ACCharacter.h"
#include "UI/Shop/ACShopWidget.h"
#include "Blueprint/UserWidget.h"
#include "AnimalCrime.h"
#include "Game/ACMainPlayerController.h"

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
    ShowInteractDebug(ACPlayer, GetName());

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

    // PlayerController를 통해 상점 토글
    AACMainPlayerController* PC = ACPlayer->GetController<AACMainPlayerController>();
    if (PC == nullptr)
    {
        UE_LOG(LogHG, Warning, TEXT("OnInteract: PlayerController is not AACMainPlayerController"));
        return;
    }

    // 서버에서 클라이언트에게 위젯 토글 명령 전송
    UE_LOG(LogHG, Log, TEXT("Server: Sending toggle command to %s"), *ACPlayer->GetName());
    PC->ClientToggleShopWidget(ShopWidgetClass);
}

