
#include "Character/ACMafiaCharacter.h"
#include "Item/ACEscapeMissionBomb.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "ACBlackMarketDealer.h"
#include "EscapeQuest/ACBombInstallArea.h"
#include "EscapeQuest/ACEscapeArea.h"
#include "Kismet/GameplayStatics.h"

#include "Game/ACMainGameState.h"
#include "AnimalCrime.h"
#include "Component/ACMoneyComponent.h"
#include "Components/CapsuleComponent.h"
#include "Component/ACDestroyableStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/ACMoneyComponent.h"

#include "AnimalCrime/AnimalCrime.h"

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
	float SuperResult = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	// 클라이언트에서 계산 시 데미지가 중첩되는 버그가 발생.
	if (HasAuthority() == false)
	{
		return 0.0f;
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
	CurrentHp -= 1.0f;
	Stat->SetCurrentHp(CurrentHp);
	AC_LOG(LogHY, Error, TEXT("My HP is %f"), Stat->GetCurrentHp());
	if (CurrentHp <= 0)
	{
		// 상태 변경
		CharacterState = ECharacterState::Stun;
		
		OnRep_CharacterState();
		if (CharacterState == ECharacterState::Stun)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
				[this]()
			{
				CharacterState = ECharacterState::Free;
				OnRep_CharacterState();
			}), 10.0, false);	
		}
		else if (CharacterState == ECharacterState::OnDamage)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
				[this]()
			{
				CharacterState = ECharacterState::Free;
				OnRep_CharacterState();
			}), 10.0, false);	
		}
	}
	else
	{
		CharacterState = ECharacterState::OnDamage;
		
		OnRep_CharacterState();
		if (CharacterState == ECharacterState::OnDamage)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
				[this]()
			{
				CharacterState = ECharacterState::Free;
				OnRep_CharacterState();
			}), 10.0, false);	
		}
	}

	return 1.0f;
}

void AACMafiaCharacter::PostNetInit()
{
	Super::PostNetInit();
}

void AACMafiaCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	TickDeltaTime -=DeltaSeconds;
	if (TickDeltaTime <= 0.0f)
	{
		AC_LOG(LogHY, Error, TEXT("My HP is %f"), Stat->GetCurrentHp());
		TickDeltaTime += 1.0f;
	}
}

void AACMafiaCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 서버만 GameState에 등록
	if (HasAuthority() == true) 
	{
		// Mafia는 100원으로 시작
		MoneyComp->InitMoneyComponent(EMoneyType::MoneyMafiaType);
		UE_LOG(LogHG, Log, TEXT("Mafia BeginPlay: MoneyMafiaType 초기화"));

		// GameState에 등록
		AACMainGameState* GS = GetWorld()->GetGameState<AACMainGameState>();
		if (GS != nullptr)
		{
			GS->MafiaPlayers.Add(this);
			AC_LOG(LogSY, Warning, TEXT("Mafia:: %d"), GS->MafiaPlayers.Num());
		}
	}
	AACMainGameState* GS = GetWorld()->GetGameState<AACMainGameState>();
	if (GS == nullptr)
	{
		return;
	}
	GS->MafiaPlayers.Add(this);
	AC_LOG(LogSY, Warning, TEXT("Mafia:: %d"), GS->MafiaPlayers.Num());
	
	// 마피아가 처음에 가지고 있는 돈 설정
	MoneyComp->InitMoneyComponent(EMoneyType::MoneyMafiaType);
	AC_LOG(LogHY, Warning, TEXT("Before HP=%f | Authority=%d"),
		Stat->GetCurrentHp(),
		HasAuthority());
	
	// 체력 설정.
	Stat->SetMaxHp(6);
	Stat->SetCurrentHp(6);
	Stat->SetArmor(0);
	AC_LOG(LogHY, Warning, TEXT("After HP=%f | Authority=%d"),
		Stat->GetCurrentHp(),
		HasAuthority());
}

bool AACMafiaCharacter::CanInteract(AACCharacter* ACPlayer)
{
	return true;
}

void AACMafiaCharacter::OnInteract(AACCharacter* ACPlayer)
{
	if (ACPlayer == nullptr)
	{
		return;
	}

	ShowInteractDebug(ACPlayer, GetName());

	// 경찰과 상호작용(신분증)
	if (EACCharacterType::Police == ACPlayer->GetCharacterType())
	{
		AC_LOG(LogSW, Log, TEXT("마피아 신분증!"));
	}
	
}

void AACMafiaCharacter::ServerFireHitscan_Implementation()
{
}

EACCharacterType AACMafiaCharacter::GetCharacterType()
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
		//참조 해제
		HandBomb->AttachedCharacter = nullptr;

		//캐릭터에서 폭탄 분리
		HandBomb->DetachFromCharacter();

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
		if (Area==nullptr) 
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
		if (Area==nullptr)
		{
			continue;
		}
		Area->SetActorHiddenInGame(!bVisible);

		Area->SetActorEnableCollision(true);
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
		UGameplayStatics::ApplyDamage(Hit.GetActor(),30.0f, GetController(),this, nullptr);
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
	DrawDebugLine(GetWorld(),MuzzleLoc, bHit ? Hit.ImpactPoint : End, LineColor, false, 2.0f, 0, 2.0f);
	if (bHit)
	{
		UE_LOG(LogTemp, Log, TEXT("Hit: %s"), *Hit.GetActor()->GetName());
		// ApplyDamage(Hit);
	}
}

void AACMafiaCharacter::FireBullet()
{
}

float AACMafiaCharacter::GetRequiredHoldTime() const
{
	return 5.0f;
}
