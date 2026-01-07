
#include "ACEscapeMissionContraband.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/WidgetComponent.h"
#include "Component/ACInteractableComponent.h"
#include "UI/Interaction/ACInteractionInfoWidget.h"
#include "Interaction/ACInteractionData.h"
#include "Character/ACMafiaCharacter.h"
#include "UI/Common/ACNotificationText.h"
#include "Game/ACMainPlayerController.h"
#include "AnimalCrime.h"

AACEscapeMissionContraband::AACEscapeMissionContraband()
{
	PrimaryActorTick.bCanEverTick = false;

	// Collision (Root)
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Sphere);

	Sphere->SetSphereRadius(20.f);
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sphere->SetGenerateOverlapEvents(false);

	// 나이아가라
	NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
	NiagaraEffect->SetupAttachment(RootComponent);
	NiagaraEffect->SetAutoActivate(true);
	NiagaraEffect->SetRelativeRotation(FRotator(0.0f, 0.0f, -20.0f));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraRef(TEXT("/Game/DrapEffet/VFX/NE_drop_effects03.NE_drop_effects03"));
	if (NiagaraRef.Succeeded())
	{
		NiagaraEffect->SetAsset(NiagaraRef.Object);
	}

	InteractBoxComponent = CreateDefaultSubobject<UACInteractableComponent>(TEXT("InteractBoxComponent"));
	InteractBoxComponent->SetupAttachment(RootComponent);
	InteractBoxComponent->SetMargin(FVector(20.f));

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

	bReplicates = true;
}

void AACEscapeMissionContraband::BeginPlay()
{
	Super::BeginPlay();

	// 루트 컴포넌트의 로컬 바운드 가져오기
	FBoxSphereBounds RootBounds = RootComponent->CalcBounds(RootComponent->GetComponentTransform());

	// BoxExtent 설정 (약간 여유 포함)
	InteractBoxComponent->SetMargin(FVector(50.f));
}

void AACEscapeMissionContraband::OnInteract(AACCharacter* ACPlayer, EInteractionKey InKey)
{
	AC_LOG(LogSY, Log, TEXT("get item"));
	AACMafiaCharacter* ACPlayerMafia = Cast<AACMafiaCharacter>(ACPlayer);
	if (ACPlayerMafia == nullptr)
	{
		return;
	}

	AACMainPlayerController* PC = Cast<AACMainPlayerController>(ACPlayer->GetController());
	if (PC == nullptr)
	{
		return;
	}

	ACPlayerMafia->AddContraband();

	PC->Client_ShowNotification(FText::FromString(TEXT("밀수품을 획득했다")));

	AC_LOG(LogSY, Log, TEXT("item Destroy"));
	Destroy();
}

EACInteractorType AACEscapeMissionContraband::GetInteractorType() const
{
	return EACInteractorType::Contraband;
}

UWidgetComponent* AACEscapeMissionContraband::GetInteractionWidget() const
{
	return InteractionWidgetComponent;
}

void AACEscapeMissionContraband::ShowInteractionHints(const TArray<class UACInteractionData*>& Interactions)
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

void AACEscapeMissionContraband::HideInteractionHints()
{
	if (!InteractionWidgetComponent) return;

	UACInteractionInfoWidget* HintWidget = Cast<UACInteractionInfoWidget>(
		InteractionWidgetComponent->GetWidget());
	if (HintWidget)
		HintWidget->HideWidget();

	InteractionWidgetComponent->SetVisibility(false);
}

