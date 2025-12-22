
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

	UE_LOG(LogHY, Error, TEXT("Damage:%f"), DamageAmount);
	if (EventInstigator != nullptr)
	{
		UE_LOG(LogHY, Error, TEXT("Constrol:%s"), *EventInstigator->GetName());
	}
	if (DamageCauser != nullptr)
	{
		UE_LOG(LogHY, Error, TEXT("Name:%s"), *DamageCauser->GetName());
	}

	float CurrentHp = Stat->GetCurrentHp();
	CurrentHp -= 1.0f;
	Stat->SetCurrentHp(CurrentHp);
	if (CurrentHp <= 0)
	{
		// 상태 변경
		CharacterState = ECharacterState::Stun;
		
		// 못 움직이도록 변경
		GetCharacterMovement()->MaxWalkSpeed = 10;
		GetCharacterMovement()->JumpZVelocity = 10;
	}

	return 1.0f;
}

void AACMafiaCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 서버만 GameState에 등록
	if (HasAuthority() == false) 
	{
		return;
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
	AC_LOG(LogHY, Warning, TEXT("Before BeginPlay HP=%f | Authority=%d"),
		Stat->GetCurrentHp(),
		HasAuthority());
	
	// 체력 설정.

	Stat->SetMaxHp(5);
	Stat->SetCurrentHp(5);
	Stat->SetArmor(0);
	AC_LOG(LogHY, Warning, TEXT("After BeginPlay HP=%f | Authority=%d"),
	Stat->GetCurrentHp(),
	HasAuthority());
	
	ForceNetUpdate();
}

bool AACMafiaCharacter::CanInteract(AACCharacter* ACPlayer)
{
	return true;
}

void AACMafiaCharacter::OnInteract(AACCharacter* ACPlayer)
{
	ShowInteractDebug(ACPlayer);
}

FString AACMafiaCharacter::GetInteractableName() const
{
	return TEXT("Mafia");
}

void AACMafiaCharacter::ServerFireHitscan_Implementation()
{
}

EACCharacterType AACMafiaCharacter::GetCharacterType()
{
	return EACCharacterType::Mafia;
}

void AACMafiaCharacter::ItemDrop(const FInputActionValue& Value)
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
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName());
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
