
#include "ACBlackMarketDealer.h"
#include "ACMafiaCharacter.h"
#include "Components/BoxComponent.h"
#include "EscapeQuest/ACBlackMarketComponent.h"
#include "Component/ACInteractableComponent.h"
#include "Game/ACMainPlayerController.h"
#include "AnimalCrime.h"

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
		0.5f,  // 0.5초 후 실행
		false // 한번 실행 후 자동 정리
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
		TArray<USkeletalMeshComponent*> MeshComponents = {
			HeadMeshComp, FaceMeshComp, TopMeshComp,
			BottomMeshComp, ShoesMeshComp, FaceAccMeshComp, GetMesh()
		};

		for (USkeletalMeshComponent* MeshComp : MeshComponents)
		{
			if (MeshComp && MafiaHighlightMaterial)
			{
				MeshComp->SetOverlayMaterial(MafiaHighlightMaterial);
			}
		}
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

	if (MafiaChar->GetContraband() <= 0)
	{
		PC->Client_ShowNotification(FText::FromString(TEXT("거래할 밀수품이 없습니다.")));
		return;
	}
	PC->Client_ShowNotification(FText::FromString(TEXT("폭탄을 획득했습니다.\n 은행에 폭탄을 설치하세요.")));

	// 밀수품 개수 차감
	MafiaChar->SubtractContraband();
	BlackMarketComponent->OpenBlackMarket(ACPlayer);
}

EACInteractorType AACBlackMarketDealer::GetInteractorType() const
{
	return EACInteractorType::BlackMarketDealer;
}

