// Fill out your copyright notice in the Description page of Project Settings.


#include "ACDestroyableObject.h"

#include "AnimalCrime.h"
#include "Character/ACMafiaCharacter.h"
#include "Component/ACDestroyableStatComponent.h"
#include "Component/ACShopComponent.h"
#include "Game/ACGameRuleManager.h"
#include "Game/ACMainGameMode.h"
#include "Item/ACItemData.h"
#include "Character/ACCharacter.h"  
#include "Components/PrimitiveComponent.h"
#include "Components/PostProcessComponent.h"


AACDestroyableObject::AACDestroyableObject()
{
	PrimaryActorTick.bCanEverTick = false;
	
	RootComp = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));;
	RootComponent = RootComp;
	
	DestroyComp = CreateDefaultSubobject<UACDestroyableStatComponent>(FName("DestroyComp"));
}

void AACDestroyableObject::BeginPlay()
{
	AC_LOG(LogHY, Log, TEXT("Begin"));
	Super::BeginPlay();

	// CustomDepth 활성화 타이머
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&AACDestroyableObject::EnableCustomDepthForMafia,
		1.5f,
		false
	);

	AC_LOG(LogHY, Log, TEXT("End"));
}

void AACDestroyableObject::PostNetInit()
{
	AC_LOG(LogHY, Log, TEXT("Begin"));
	Super::PostNetInit();
	AC_LOG(LogHY, Log, TEXT("End"));
}

void AACDestroyableObject::PostInitializeComponents()
{
	AC_LOG(LogHY, Log, TEXT("Begin"));
	Super::PostInitializeComponents();
	AC_LOG(LogHY, Log, TEXT("End"));
}

float AACDestroyableObject::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	AC_LOG(LogHY, Log, TEXT("둘 다 호출?"));
	
	if (IsValid(DamageCauser) == false)
	{
		AC_LOG(LogHY, Log, TEXT("IsValid fail"));
		return 0.0f;
	}
	
	AACMafiaCharacter* MafiaPawn = Cast<AACMafiaCharacter>(DamageCauser);
	if (MafiaPawn == nullptr)
	{
		AC_LOG(LogHY, Log, TEXT("MafiaPawn is nullptr"));
		return 0.0f;
	}
	
	UACShopComponent* ShopComp = MafiaPawn->FindComponentByClass<UACShopComponent>();
	if (ShopComp == nullptr)
	{
		AC_LOG(LogHY, Log, TEXT("ShopComp is nullptr"));
		return 0.0f;
	}

	// 현재 장착된 무기 확인
	UACItemData* EquippedWeapon = ShopComp->EquippedWeapon;
	if (EquippedWeapon == nullptr)
	{
		AC_LOG(LogHY, Log, TEXT("EquippedWeapon is nullptr"));
		return 0.0f;
	}
	if (EquippedWeapon->ItemType != EItemType::Weapon)
	{
		AC_LOG(LogTemp, Log, TEXT(">>> Weapon Equipped: %s"), *EquippedWeapon->ItemName.ToString());
		return 0.0f;
	}
	AC_LOG(LogTemp, Log, TEXT(">>> Weapon Equipped: %s"), *EquippedWeapon->ItemName.ToString());
	
	// 데미지 누적
	DestroyComp->OnDamaged(DamageAmount * 10);
	float CurrentHP = DestroyComp->GetCurrentHp();
	if (CurrentHP <= 0.0f)
	{
		AC_LOG(LogHY, Error, TEXT("과연 클라도?"));
		HandleDestroyed(EventInstigator);
		Destroy();
	}
	
	return 1.0f; 
}

void AACDestroyableObject::Tick(float DeltaTime)
{
	AC_LOG(LogHY, Log, TEXT("Begin"));
	Super::Tick(DeltaTime);
	AC_LOG(LogHY, Log, TEXT("End"));
}

void AACDestroyableObject::HandleDestroyed(AController* InstigatorController)
{
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode == nullptr)
	{
		AC_LOG(LogHY, Log, TEXT("GameMode is nullptr"));
		return ;
	}
	AACMainGameMode* ACGameMode = Cast<AACMainGameMode>(GameMode);
	if (ACGameMode == nullptr)
	{
		AC_LOG(LogHY, Log, TEXT("ACGameMode cast failed"));
		return;
	}
	
	ACGameMode->UpdateGameScoreFromMafia(EMafiaAction::DestroyObject, EarnScore);
}

void AACDestroyableObject::EnableCustomDepthForMafia()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC == nullptr)
	{
		UE_LOG(LogHG, Error, TEXT("PC is nullptr"));
		return;
	}

	// 로컬 컨트롤러인지 확인 (중요!)
	if (!PC->IsLocalController())
	{
		return;
	}

	AACCharacter* LocalChar = Cast<AACCharacter>(PC->GetPawn());
	if (LocalChar == nullptr)
	{
		UE_LOG(LogHG, Error, TEXT("LocalChar is nullptr"));
		return;
	}

	// 마피아만 Custom Depth 활성화
	if (LocalChar->GetCharacterType() == EACCharacterType::Mafia)
	{
		UE_LOG(LogHG, Warning, TEXT("DestroyableObject (%s): Enabling CustomDepth with Stencil=1"), *GetName());

		// 모든 Mesh Component에 Custom Depth 활성화
		TArray<UMeshComponent*> MeshComponents;
		GetComponents<UMeshComponent>(MeshComponents);

		for (UMeshComponent* MeshComp : MeshComponents)
		{
			if (MeshComp != nullptr)
			{
				//MeshComp->SetRenderCustomDepth(true);
				//MeshComp->SetCustomDepthStencilValue(1);  // Stencil = 1 (DestroyableObject용)

				// CustomDepth 렌더링 강제 활성화
				MeshComp->SetRenderCustomDepth(true);
				MeshComp->SetCustomDepthStencilValue(1);

				// CustomDepth가 depth buffer에 제대로 쓰이도록 설정
				MeshComp->bRenderCustomDepth = true;

				// Mesh가 depth에 제대로 렌더링되도록 설정
				MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			}
		}
	}
	else
	{
		UE_LOG(LogHG, Warning, TEXT("DestroyableObject (%s): Not Mafia, skipping CustomDepth"), *GetName());
	}
}