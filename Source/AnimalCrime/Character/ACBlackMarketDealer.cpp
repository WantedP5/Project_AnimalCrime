
#include "ACBlackMarketDealer.h"
#include "ACMafiaCharacter.h"
#include "Components/BoxComponent.h"
#include "EscapeQuest/ACBlackMarketComponent.h"
#include "Component/ACInteractableComponent.h"
#include "Game/ACMainPlayerController.h"
#include "AnimalCrime.h"
#include "Components/PostProcessComponent.h"

AACBlackMarketDealer::AACBlackMarketDealer()
{
	//HeadMeshComp->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Hairstyle_male_010.SK_Hairstyle_male_010")));
	//TopMesh->SetSkeletalMesh(nullptr);
	//BottomMesh->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Costume_6_001.SK_Costume_6_001")));
	BlackMarketComponent = CreateDefaultSubobject<UACBlackMarketComponent>(TEXT("BlackMarketComponent"));

}

void AACBlackMarketDealer::BeginPlay()
{
    Super::BeginPlay();

    // 약간의 딜레이 후 하이라이트 적용 (플레이어 스폰 타이밍 고려)
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        this,
        &AACBlackMarketDealer::UpdateHighlightForLocalPlayer,
        0.5f,
        false
    );
}

void AACBlackMarketDealer::UpdateHighlightForLocalPlayer()
{
    // 로컬 플레이어 가져오기
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC == nullptr)
    {
        AC_LOG(LogHG, Error, TEXT("PC is nullptr"));
        return;
    }

    AACCharacter* LocalChar = Cast<AACCharacter>(PC->GetPawn());
    if (LocalChar == nullptr)
    {
        AC_LOG(LogHG, Error, TEXT("LocalChar is nullptr"));
        return;
    }

    // 마피아만 하이라이트 적용
    if (LocalChar->GetCharacterType() == EACCharacterType::Mafia)
    {
        UE_LOG(LogHG, Warning, TEXT("BlackMarketDealer: Applying highlights for Mafia"));

        TArray<USkeletalMeshComponent*> MeshComponents = {
            HeadMeshComp, FaceMeshComp, TopMeshComp,
            BottomMeshComp, ShoesMeshComp, FaceAccMeshComp, GetMesh()
        };

        // 1. Overlay Material 적용 (흰색 외곽선, 건물에 가려짐)
        for (USkeletalMeshComponent* MeshComp : MeshComponents)
        {
            if (MeshComp && MafiaHighlightMaterial)
            {
                MeshComp->SetOverlayMaterial(MafiaHighlightMaterial);
            }
        }

        // 2. Custom Depth 활성화 (PPM_Highlight용, 건물 뒤 노란색)
        for (USkeletalMeshComponent* MeshComp : MeshComponents)
        {
            if (MeshComp)
            {
                MeshComp->SetRenderCustomDepth(true);
                MeshComp->SetCustomDepthStencilValue(2);  // Stencil = 2 (BlackMarketDealer용)
            }
        }

        UE_LOG(LogHG, Warning, TEXT("BlackMarketDealer: Overlay + CustomDepth(Stencil=2) applied"));
    }
}

void AACBlackMarketDealer::OnInteract(AACCharacter* ACPlayer, EInteractionKey InKey)
{
	if (BlackMarketComponent == nullptr)
	{
		return;
	}
	AACMafiaCharacter* MafiaChar = Cast<AACMafiaCharacter>(ACPlayer);
	if (MafiaChar == nullptr)
	{
		return;
	}

	AACMainPlayerController* PC = Cast<AACMainPlayerController>(ACPlayer->GetController());
	if (PC == nullptr)
	{
		AC_LOG(LogSY, Log, TEXT("PC is nullptr"));
		return;
	}

	// ===== 수정: Character에서 직접 체크 =====
	if (MafiaChar->GetContraband() <= 0)
	{
		PC->Client_ShowNotification(FText::FromString(TEXT("거래할 밀수품이 없습니다.")), ENotificationType::None);
		return;
	}
	PC->Client_ShowNotification(FText::FromString(TEXT("폭탄을 획득했습니다.\n 은행에 폭탄을 설치하세요.")), ENotificationType::None);

	// 밀수품 개수 차감
	MafiaChar->SubtractContraband();

	BlackMarketComponent->OpenBlackMarket(ACPlayer);
}

EACInteractorType AACBlackMarketDealer::GetInteractorType() const
{
	return EACInteractorType::BlackMarketDealer;
}

