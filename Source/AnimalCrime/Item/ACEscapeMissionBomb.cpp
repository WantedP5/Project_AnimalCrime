
#include "ACEscapeMissionBomb.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Character/ACTestMafiaCharacter.h"
#include "Net/UnrealNetwork.h"
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

	InteractBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBoxComponent"));
	InteractBoxComponent->SetupAttachment(RootComponent);

	// 네트워크 복제 설정. 서버에 이 액터가 생성되면 클라이언트로 복제됨, 삭제도 동기화
	bReplicates = true;

	// 이동 복제 활성화
	SetReplicateMovement(true);
}

void AACEscapeMissionBomb::BeginPlay()
{
	Super::BeginPlay();
	InteractBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AACEscapeMissionBomb::OnInteractTriggerOverlapBegin);
	InteractBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AACEscapeMissionBomb::OnInteractTriggerOverlapEnd);

	//// 루트 컴포넌트의 로컬 바운드 가져오기
	//FBoxSphereBounds RootBounds = BombMeshComp->Bounds;

	//// BoxExtent 설정 (약간 여유 포함)
	//FVector Margin(150.f, 150.f, 150.f); // 필요에 따라 조정
	//InteractBoxComponent->SetBoxExtent(RootBounds.BoxExtent + Margin);

	//// 박스 위치 루트에 맞추기
	//InteractBoxComponent->SetRelativeLocation(FVector::ZeroVector);

	// 루트 컴포넌트의 로컬 바운드 가져오기
	FBoxSphereBounds RootBounds = RootComponent->CalcBounds(RootComponent->GetComponentTransform());

	// BoxExtent 설정 (약간 여유 포함)
	FVector Margin(50.f, 50.f, 50.f);
	InteractBoxComponent->SetBoxExtent(RootBounds.BoxExtent + Margin);

	// 박스 위치 루트에 맞추기
	InteractBoxComponent->SetRelativeLocation(FVector::ZeroVector);
}

void AACEscapeMissionBomb::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AACEscapeMissionBomb, AttachedCharacter);
}

void AACEscapeMissionBomb::OnInteractTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AACTestMafiaCharacter* TestMafiaChar = Cast<AACTestMafiaCharacter>(OtherActor);
	if (TestMafiaChar == nullptr)
	{
		return;
	}

	AC_LOG(LogSY, Log, TEXT("InteractBox Begin"));
	TestMafiaChar->InteractBomb = this;
}

void AACEscapeMissionBomb::OnInteractTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AACTestMafiaCharacter* TestMafiaChar = Cast<AACTestMafiaCharacter>(OtherActor);
	if (TestMafiaChar == nullptr)
	{
		return;
	}

	AC_LOG(LogSY, Log, TEXT("InteractBox End"));
	TestMafiaChar->InteractBomb = nullptr;
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

