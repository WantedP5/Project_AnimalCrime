
#include "ACEscapeMissionBomb.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Character/ACMafiaCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Component/ACInteractableComponent.h"
#include "Game/ACMainGameState.h"

#include "AnimalCrime.h"

AACEscapeMissionBomb::AACEscapeMissionBomb()
{

	RootComp = CreateDefaultSubobject<UBoxComponent>(TEXT("RootBox"));
	RootComponent = RootComp;

	// BoxComponent 콜리전 설정
	RootComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RootComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	RootComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);  // 캐릭터와 Block
	
	RootComp->SetBoxExtent(FVector(50.f, 50.f, 50.f));

	BombMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bomb"));
	BombMeshComp->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	BombMeshComp->SetupAttachment(RootComponent);
	BombMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BombMeshComp->SetRelativeLocation(FVector(0.f, 0.f, -10.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BombMeshRef(TEXT("/Game/wooden_barrier_setlow_poly/StaticMeshes/Object_12.Object_12"));
	if (BombMeshRef.Succeeded())
	{
		BombMeshComp->SetStaticMesh(BombMeshRef.Object);
	}

	//물리 및 중력 설정
	//BombMeshComp->SetSimulatePhysics(true);	// 물리 시뮬레이션 활성화
	//BombMeshComp->SetEnableGravity(true);	// 중력 활성화
	//BombMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//BombMeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));

	InteractBoxComponent = CreateDefaultSubobject<UACInteractableComponent>(TEXT("InteractBoxComponent"));
	InteractBoxComponent->SetupAttachment(RootComponent);
	InteractBoxComponent->SetMargin(FVector(20.f));

	// 네트워크 복제 설정. 서버에 이 액터가 생성되면 클라이언트로 복제됨, 삭제도 동기화
	bReplicates = true;

	// 이동 복제 활성화
	SetReplicateMovement(true);
}

void AACEscapeMissionBomb::BeginPlay()
{
	Super::BeginPlay();

	// 루트 컴포넌트의 로컬 바운드 가져오기
	FBoxSphereBounds RootBounds = RootComponent->CalcBounds(RootComponent->GetComponentTransform());

	// BoxExtent 설정 (약간 여유 포함)
	//FVector Margin(50.f, 50.f, 50.f);
	InteractBoxComponent->SetMargin(FVector(50.f));
	//InteractBoxComponent->SetBoxExtent(RootBounds.BoxExtent + Margin);

	// 박스 위치 루트에 맞추기
	InteractBoxComponent->SetRelativeLocation(FVector::ZeroVector);
}

void AACEscapeMissionBomb::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AACEscapeMissionBomb, AttachedCharacter);
}

void AACEscapeMissionBomb::OnRep_AttachedCharacter()
{
	if (AttachedCharacter != nullptr)
	{
		AttachToCharacter();
	}
	else
	{
		DetachFromCharacter();
	}
}

void AACEscapeMissionBomb::AttachToCharacter()
{
	if (AttachedCharacter != nullptr)
	{
		//폭탄 물리 끄기
		//BombMeshComp->SetSimulatePhysics(false);
		SetReplicateMovement(false);
		SetActorEnableCollision(false);

		//캐릭터 소켓에 폭탄 부착
		USkeletalMeshComponent* CharacterMeshComp = AttachedCharacter->GetMesh();
		if (CharacterMeshComp == nullptr)
		{
			return;
		}
		AttachToComponent(
			CharacterMeshComp,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			TEXT("CarryingSpine")
		);

		AC_LOG(LogSY, Log, TEXT("Bomb Attach to Character"));
	}
}

void AACEscapeMissionBomb::DetachFromCharacter()
{
	//캐릭터 소켓에서 폭탄 분리
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	//폭탄 물리 켜기
	//BombMeshComp->SetSimulatePhysics(true);
	SetReplicateMovement(true);
	SetActorEnableCollision(true);

	AC_LOG(LogSY, Log, TEXT("Bomb Detach to Character"));
}

/**
	@brief  폭탄은 마피아만 잡아야함.
	@param  Interactor - 겹친 대상
	@retval            - 상호작용 가능 여부
**/
bool AACEscapeMissionBomb::CanInteract(AACCharacter* ACPlayer)
{
	if (ACPlayer == nullptr)
	{
		AC_LOG(LogSW, Log, TEXT("Sorry aaaaa"));
		return false;
	}
	if (ACPlayer->GetCharacterType() != EACCharacterType::Mafia)
	{
		AC_LOG(LogSW, Log, TEXT("Sorry Only For MAFIA!!!!!"));
		return false;
	}

	AC_LOG(LogSW, Log, TEXT("Bomb - Mafia Contacted!!"));
	return true;
}

void AACEscapeMissionBomb::OnInteract(AACCharacter* ACPlayer)
{
	ShowInteractDebug(ACPlayer);

	AACMafiaCharacter* ACPlayerMafia = Cast<AACMafiaCharacter>(ACPlayer);

	if (ACPlayerMafia == nullptr)
	{
		return;
	}

	// 1. 캐릭터가 이미 폭탄을 들고 있으면 불가
	if (ACPlayerMafia->HandBomb != nullptr)
	{
		AC_LOG(LogSY, Warning, TEXT("Character Already Has a Bomb"));
		return;
	}

	// 2. 폭탄을 캐릭터에게 부착
	AttachedCharacter = ACPlayerMafia;
	ACPlayerMafia->HandBomb = this;

	AttachToCharacter();

	// 3. 설치 가능 구역 보이기
	AACMainGameState* GS = ACPlayerMafia->GetWorld()->GetGameState<AACMainGameState>();
	if (GS && GS->EscapeState == EEscapeState::DeliverBomb)
	{
		ACPlayerMafia->ClientSetBombAreaVisible(true);
	}

	AC_LOG(LogSY, Log, TEXT("Bomb Interact Success"));

	AttachToCharacter();
}

FString AACEscapeMissionBomb::GetInteractableName() const
{
	return TEXT("Bomb");
}