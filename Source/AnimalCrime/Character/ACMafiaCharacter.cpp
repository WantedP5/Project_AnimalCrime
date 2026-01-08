
#include "Character/ACMafiaCharacter.h"
#include "Item/ACEscapeMissionBomb.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "ACBlackMarketDealer.h"
#include "EscapeQuest/ACBombInstallArea.h"
#include "EscapeQuest/ACEscapeArea.h"
#include "Kismet/GameplayStatics.h"

#include "Game/ACMainGameState.h"
#include "Game/ACMainGameMode.h"
#include "AnimalCrime.h"
#include "Component/ACMoneyComponent.h"
#include "Components/CapsuleComponent.h"
#include "Component/ACDestroyableStatComponent.h"
#include "Component/ACShopComponent.h"
#include "DSP/LFO.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Game/ACMainPlayerController.h"
#include "Item/ACItemData.h"
#include "Sound/SoundBase.h"

AACMafiaCharacter::AACMafiaCharacter()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MafiaCollision"));

	Stat = CreateDefaultSubobject<UACDestroyableStatComponent>(TEXT("StatComponent"));
}

void AACMafiaCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AACMafiaCharacter, HandBomb);
}

float AACMafiaCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 감옥 상태에서는 맞아서는 안됌.
	if (CharacterState == ECharacterState::OnDamage ||
		CharacterState == ECharacterState::Stun ||
		CharacterState == ECharacterState::Prison || 
		CharacterState == ECharacterState::PrisonEscape || 
		CharacterState == ECharacterState::OnInteract)
	{
		return 0.0;
	}
	
	float SuperResult = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 피격 사운드 재생
	if (HitSound && DamageAmount > 0.0f)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}

	// 권한있는 APawn만 계산해야 함.
	if (HasAuthority() == false)
	{
		return 0.0f;
	}

	// 서버에서만 실행되는 영역에 Multicast 호출 
	if (DamageAmount > 0.0f)
	{
		MulticastPlayHitEffect(10.f);  // 모든 클라이언트에 전파
	}

	AC_LOG(LogHY, Error, TEXT("Damage:%f"), DamageAmount);
	if (EventInstigator != nullptr)
	{
		AC_LOG(LogHY, Error, TEXT("MyName:%s EventInstigator:%s"), *GetName(), *EventInstigator->GetName());
	}
	if (DamageCauser != nullptr)
	{
		AC_LOG(LogHY, Error, TEXT("MyName:%s DamageCauser:%s"), *GetName(), *DamageCauser->GetName());
	}

	float CurrentHp = Stat->GetCurrentHp();
	CurrentHp -= DamageAmount;
	Stat->SetCurrentHp(CurrentHp);
	//AC_LOG(LogHY, Error, TEXT("My HP is %f"), Stat->GetCurrentHp());

	FTimerDelegate TimerDelegate;
	if (CurrentHp <= 0)
	{
		// 상태 변경
		CharacterState = ECharacterState::Stun;
		OnRep_CharacterState();
		// todo: SetCharacterState(ECharacterState::Stun) ?????
		
		if (CharacterState == ECharacterState::Stun)
		{
			TimerDelegate.BindUObject(this, &AACMafiaCharacter::UpdateCharacterStatusRevive);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 10.0, false);
		}
	}
	else
	{
		CharacterState = ECharacterState::OnDamage;
		OnRep_CharacterState();
		// todo: SetCharacterState(ECharacterState::OnDamage) ?????

		if (CharacterState == ECharacterState::OnDamage)
		{
			TimerDelegate.BindUObject(this, &AACMafiaCharacter::UpdateCharacterStatusFree);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 10.0, false);
		}
	}

	return DamageAmount;
}

void AACMafiaCharacter::PostNetInit()
{
	Super::PostNetInit();
}

void AACMafiaCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/*TickDeltaTime -=DeltaSeconds;
	if (TickDeltaTime <= 0.0f)
	{
		AC_LOG(LogHY, Error, TEXT("My HP is %f"), Stat->GetCurrentHp());
		TickDeltaTime += 1.0f;
	}*/
}

void AACMafiaCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AC_LOG(LogHY, Warning, TEXT("AACMafiaCharacter::EndPlay"));
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	Super::EndPlay(EndPlayReason);
}

void AACMafiaCharacter::UpdateCharacterStatusFree()
{
	if (IsValid(this) == false)
	{
		AC_LOG(LogHY, Error, TEXT("this가 올바르지 않습니다."));
		return;
	}

	AC_LOG(LogHY, Error, TEXT("상태가 변경되었습니다."));
	// false로 미리 바꿔야 계산이 된다.
	bOnDamage = false;
	SetCharacterState(ECharacterState::Free);
	OnRep_CharacterState();
}

void AACMafiaCharacter::UpdateCharacterStatusRevive()
{
	if (IsValid(this) == false)
	{
		AC_LOG(LogHY, Error, TEXT("this가 올바르지 않습니다."));
		return;
	}
	AC_LOG(LogHY, Error, TEXT("상태가 변경되었습니다."));
	bStun = false;
	SetCharacterState(ECharacterState::Free);
	OnRep_CharacterState();
	if (HasAuthority() == true)
	{
		Stat->SetCurrentHp(MaxHpData);
	}
}

void AACMafiaCharacter::BeginPlay()
{
	AC_LOG(LogHY, Error, TEXT("Begin"));
	Super::BeginPlay();

	// 서버만 GameState에 등록
	if (HasAuthority() == false)
	{
		AC_LOG(LogHY, Error, TEXT("HasAuthority is false"));
		return;
	}

	AACMainGameMode* ACGameMode = GetWorld()->GetAuthGameMode<AACMainGameMode>();
	// 게임 모드 확인
	if (ACGameMode == nullptr)
	{
		AC_LOG(LogHY, Warning, TEXT("ACGameMode is null"));
		return;
	}

	/*
	*	랜덤 옷 입히기 로직
	*/
	FOutfitCombo OutFit = ACGameMode->GetClothesFromPool();

	HeadMeshReal = OutFit.HairAsset.LoadSynchronous();
	OnRep_HeadMesh();

	FaceMeshReal = OutFit.FaceAsset.LoadSynchronous();
	OnRep_FaceMesh();
	TopMeshReal = OutFit.TopAsset.LoadSynchronous();
	OnRep_TopMesh();
	BottomMeshReal = OutFit.BottomAsset.LoadSynchronous();
	OnRep_BottomMesh();
	ShoesMeshReal = OutFit.ShoesAsset.LoadSynchronous();
	OnRep_ShoesMesh();
	FaceAccMeshReal = OutFit.FaceAccAsset.LoadSynchronous();
	OnRep_FaceAccMesh();

	// Mafia는 100원으로 시작
	MoneyComp->InitMoneyComponent(EMoneyType::MoneyMafiaType);

	// GameState에 등록
	AACMainGameState* ACGameState = GetWorld()->GetGameState<AACMainGameState>();
	if (ACGameState == nullptr)
	{
		return;
	}
	ACGameState->MafiaPlayers.Add(this);
	AC_LOG(LogSY, Warning, TEXT("Mafia:: %d"), ACGameState->MafiaPlayers.Num());

	// 마피아가 처음에 가지고 있는 돈 설정
	MoneyComp->InitMoneyComponent(EMoneyType::MoneyMafiaType);
	AC_LOG(LogHY, Warning, TEXT("Before HP=%f | Authority=%d"),
		Stat->GetCurrentHp(),
		HasAuthority());

	// 체력 설정.
	Stat->SetMaxHp(5);
	Stat->SetCurrentHp(5);
	Stat->SetArmor(0);
	AC_LOG(LogHY, Warning, TEXT("After HP=%f | Authority=%d"), Stat->GetCurrentHp(), HasAuthority());

	// Tax 설정
	ChangeTax(60);

	AC_LOG(LogHY, Error, TEXT("End"));
}

void AACMafiaCharacter::OnInteract(AACCharacter* ACPlayer, EInteractionKey InKey)
{
	if (ACPlayer == nullptr)
	{
		return;
	}

	//// 경찰과 상호작용(신분증)
	//if (this->CharacterState == ECharacterState::Free)
	//{
	//	AC_LOG(LogSW, Log, TEXT("마피아 신분증!"));
	//}

	AACMainPlayerController* PC = Cast<AACMainPlayerController>(ACPlayer->GetController());
	if (PC == nullptr)
	{
		AC_LOG(LogSY, Log, TEXT("PC is nullptr"));
		return;
	}

		//todo: DB로 교체
	AACMainGameMode* GM = GetWorld()->GetAuthGameMode<AACMainGameMode>();
	switch (InKey)
	{
	case EInteractionKey::E:
		AC_LOG(LogSW, Log, TEXT("마피아 신분증!"))
			PC->Client_ShowNotification(FText::FromString(TEXT("마피아")));
			break;
	case EInteractionKey::R:
		// 경찰과 상호작용(투옥)
		//if (this->CharacterState == ECharacterState::Stun)
		//{
		//	
		//}
		// 경찰과 상호작용(투옥)
		//AACMainGameMode* GM = GetWorld()->GetAuthGameMode<AACMainGameMode>();
		if (GM == nullptr)
		{
			return;
		}
		GM->ImprisonCharacter(this);  // GameMode에 캡슐화 함수 사용

		break;
	case EInteractionKey::T: break;

	}

}

EACInteractorType AACMafiaCharacter::GetInteractorType() const
{
	return EACInteractorType::Mafia;
}

void AACMafiaCharacter::MulticastPlayHitEffect_Implementation(float Duration)
{
	PlayHitEffect(Duration);
}

void AACMafiaCharacter::ServerFireHitscan_Implementation()
{
}

EACCharacterType AACMafiaCharacter::GetCharacterType() const
{
	return EACCharacterType::Mafia;
}

void AACMafiaCharacter::ItemDrop()
{
	AC_LOG(LogSY, Log, TEXT("ItemDrop Key!!"));
	ServerItemDrop(); //서버에 알림.
}

void AACMafiaCharacter::ServerItemDrop_Implementation()
{
	if (HandBomb != nullptr)
	{
		//캐릭터에서 폭탄 분리
		HandBomb->DetachFromCharacter();

		//참조 해제
		HandBomb->AttachedCharacter = nullptr;

		// Impulse 적용은 서버가 직접 해야 하므로 여기서 처리
		//UStaticMeshComponent* MeshComp = HandBomb->GetBombMeshComp();
		//if (MeshComp)
		//{
		//    FVector ThrowDir = GetActorForwardVector() + FVector(0, 0, 0.3f);
		//    MeshComp->AddImpulse(ThrowDir * 300.f, NAME_None, true);
		//}

		HandBomb = nullptr;

		//폭탄 설치 가능 구역 숨기기

		ClientSetBombAreaVisible(false);
		AC_LOG(LogSY, Log, TEXT("Bomb dropped"));
	}
	else
	{
		AC_LOG(LogSY, Warning, TEXT("No bomb to drop"));
	}
}

void AACMafiaCharacter::ClientSetBombAreaVisible_Implementation(bool bVisible)
{
	AACMainGameState* GS = GetWorld()->GetGameState<AACMainGameState>();
	if (GS == nullptr)
	{
		return;
	}
	AC_LOG(LogSY, Log, TEXT("%d"), GS->BombAreas.Num());
	for (AACBombInstallArea* Area : GS->BombAreas)
	{
		if (Area == nullptr)
		{
			continue;
		}

		Area->SetActorHiddenInGame(!bVisible);

		AC_LOG(LogSY, Log, TEXT("ClientSetBombAreaVisible"));
	}
}

void AACMafiaCharacter::ClientSetEscapeAreaVisible_Implementation(bool bVisible)
{
	AACMainGameState* GS = GetWorld()->GetGameState<AACMainGameState>();
	if (GS == nullptr)
	{
		return;
	}
	AC_LOG(LogSY, Log, TEXT("%d"), GS->EscapeAreas.Num());
	for (AACEscapeArea* Area : GS->EscapeAreas)
	{
		if (Area == nullptr)
		{
			continue;
		}
		Area->SetActorHiddenInGame(!bVisible);

		AC_LOG(LogSY, Log, TEXT("ClientSetBombAreaVisible"));
	}
}

void AACMafiaCharacter::OnRep_HandBomb()
{
	if (HandBomb)
	{
		AC_LOG(LogSY, Log, TEXT("OnRep_HandBomb: Now holding bomb"));
	}
	else
	{
		AC_LOG(LogSY, Log, TEXT("OnRep_HandBomb: No longer holding bomb"));
	}
}

void AACMafiaCharacter::AttackHitCheck()
{
	// AACCharacter 클래스(Empty)
	Super::AttackHitCheck();

	// FireHitscan();
	// 캡슐 크기
	float CapsuleRadius = 30.0f;
	float CapsuleHalfHeight = 60.0f;

	// 트레이스 길이
	float TraceDistance = 200.0f;

	// 시작 위치 = 캐릭터 위치
	FVector Start = GetActorLocation();

	// 끝 위치 = 캐릭터 앞 방향 * 거리
	FVector Forward = GetActorForwardVector();
	FVector End = Start + Forward * TraceDistance;

	// 충돌 파라미터 설정
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);   // 자기 자신 무시
	Params.bTraceComplex = false;
	Params.bReturnPhysicalMaterial = false;

	FHitResult Hit;

	// bool bHit = GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_GameTraceChannel2 | ECC_GameTraceChannel4, FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight), Params);

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel6);
	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel7);
	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel8);

	bool bHit = GetWorld()->SweepSingleByObjectType(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ObjectParams,
		FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),
		Params
	);

	// 디버그: 캡슐 그리기
	DrawDebugCapsule(GetWorld(), (Start + End) * 0.5f, CapsuleHalfHeight, CapsuleRadius, FRotationMatrix::MakeFromZ(End - Start).ToQuat(), bHit ? FColor::Red : FColor::Green, false, 1.0f);

	if (bHit)
	{
		AC_LOG(LogHY, Warning, TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName());
		
		UGameplayStatics::ApplyDamage(Hit.GetActor(), 30.0f, GetController(), this, nullptr);
	}
}

// void AACMafiaCharacter::FireHitscan()
// {
// 	FVector Start = GetActorLocation();
// 	FVector End = Start + (GetActorForwardVector() * 2000.f);
//
// 	FHitResult Hit;
//
// 	FCollisionObjectQueryParams ObjectParams;
// 	
// 	ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
// 	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
// 	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
// 	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel6);
// 	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel7);
// 	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel8);
//
// 	FCollisionQueryParams QueryParams;
// 	QueryParams.AddIgnoredActor(this);
// 	QueryParams.AddIgnoredActor(GetOwner());
// 	QueryParams.bReturnPhysicalMaterial = true;
//
// 	bool bHit = GetWorld()->LineTraceSingleByObjectType(
// 		Hit,
// 		Start,
// 		End,
// 		ObjectParams,
// 		QueryParams
// 	);
//
// 	//
// 	FColor LineColor = bHit ? FColor::Red : FColor::Green;
// 	DrawDebugLine(
// 		GetWorld(),
// 		Start,
// 		bHit ? Hit.ImpactPoint : End,
// 		LineColor,
// 		false,     // 지속 시간 (false = 1 프레임만)
// 		2.0f,      // 디버그 표시 시간
// 		0,
// 		2.0f       // 선 굵기
// 	);
//
// 	//
// 	if (bHit)
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("Hit: %s"), *Hit.GetActor()->GetName());
// 		//ApplyDamage(Hit);
// 	}
// }

void AACMafiaCharacter::FireHitscan()
{
	FVector CameraLoc;
	FRotator CameraRot;
	float MaxDistance = 1000;
	GetOwner()->GetActorEyesViewPoint(CameraLoc, CameraRot);
	FVector TraceEnd = CameraLoc + (CameraRot.Vector() * MaxDistance);

	// 총구 위치
	FVector MuzzleLoc = GetMesh()->GetSocketLocation("RightHandSocket");

	FVector ShootDir = (TraceEnd - MuzzleLoc).GetSafeNormal();
	FVector End = MuzzleLoc + (ShootDir * MaxDistance);

	FHitResult Hit;

	FCollisionObjectQueryParams ObjectParams;

	ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel6);
	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel7);
	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel8);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.bReturnPhysicalMaterial = true;

	bool bHit = GetWorld()->LineTraceSingleByObjectType(Hit, MuzzleLoc, End, ObjectParams, QueryParams);

	FColor LineColor = bHit ? FColor::Red : FColor::Green;
	DrawDebugLine(GetWorld(), MuzzleLoc, bHit ? Hit.ImpactPoint : End, LineColor, false, 2.0f, 0, 2.0f);
	if (bHit)
	{
		UE_LOG(LogTemp, Log, TEXT("Hit: %s"), *Hit.GetActor()->GetName());
		// ApplyDamage(Hit);
	}
}

void AACMafiaCharacter::FireBullet()
{
}

void AACMafiaCharacter::CalculateTax()
{
	if (IsValid(this) == false)
	{
		AC_LOG(LogHY, Error, TEXT("this is not Valid"));
		return;
	}

	// 돈 추가 로직
	MoneyComp->LoseMoney(5);
}

void AACMafiaCharacter::ChangeTax(float InTimeRate)
{
	GetWorldTimerManager().ClearTimer(TaxTimerHandle);

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &AACMafiaCharacter::CalculateTax);
	TaxTimeRate = InTimeRate;
	GetWorld()->GetTimerManager().SetTimer(TaxTimerHandle, TimerDelegate, TaxTimeRate, true);
}

float AACMafiaCharacter::GetCurrentHP() const
{
	return Stat->GetCurrentHp();
}

void AACMafiaCharacter::ExcuteEscape()
{
	if (EscapeCount <= 0)
	{
		return;
	}
	
	ServerEscape();
}

void AACMafiaCharacter::ServerEscape_Implementation()
{
	EscapeCount = EscapeCount - 1;
	AC_LOG(LogHY, Error, TEXT("Log HI"));
	SetCharacterState(ECharacterState::Free);
}

