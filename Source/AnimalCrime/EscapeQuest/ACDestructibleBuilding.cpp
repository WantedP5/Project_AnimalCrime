
#include "EscapeQuest/ACDestructibleBuilding.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AACDestructibleBuilding::AACDestructibleBuilding()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(false);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Static Mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BuildingRef(TEXT("/Game/Lowpoly_City/Meshes/SM_Building_Skyscraper_02.SM_Building_Skyscraper_02"));
	if (BuildingRef.Succeeded())
	{
		StaticMesh->SetStaticMesh(BuildingRef.Object);
	}

	// Fracture Mesh
	FractureMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("FractureMesh"));
	FractureMesh->SetupAttachment(RootComponent);
	FractureMesh->SetVisibility(false);

	static ConstructorHelpers::FObjectFinder<UGeometryCollection> GC_Building(
		TEXT("/Game/Project/EscapeQuest/GC_SM_Building_Skyscraper.GC_SM_Building_Skyscraper")
	);

	if (GC_Building.Succeeded())
	{
		FractureCollectionAsset = GC_Building.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_Destroy(
		TEXT("/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_B.P_Explosion_Big_B")
	);

	if (PS_Destroy.Succeeded())
	{
		DestroyEffect = PS_Destroy.Object;
	}
}

void AACDestructibleBuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AACDestructibleBuilding, bDestroyed);
}

void AACDestructibleBuilding::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh->SetVisibility(true);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (FractureCollectionAsset)
	{
		FractureMesh->SetRestCollection(FractureCollectionAsset);
	}
	FractureMesh->SetVisibility(false);
}

void AACDestructibleBuilding::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//물리 설정
	StaticMesh->SetCollisionProfileName(TEXT("BlockAll"));
	FractureMesh->SetSimulatePhysics(false);
	FractureMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AACDestructibleBuilding::DestroyBuilding()
{
	if (HasAuthority() == false)
	{
		return;
	}

	bDestroyed = true;   // 파괴를 클라이언트에 알림
	OnRep_Destroyed();   // 서버도 즉시 실행
}

void AACDestructibleBuilding::OnRep_Destroyed()
{
	// Static Mesh 제거
	StaticMesh->SetVisibility(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 프랙처 메시 활성화
	FractureMesh->SetVisibility(true);
	FractureMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// 캐릭터, 카메라 무시
	FractureMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Ignore); // 마피아
	FractureMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel7, ECR_Ignore); // 경찰
	FractureMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel8, ECR_Ignore); // 시민

	FractureMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	FractureMesh->SetSimulatePhysics(true);

	// 파괴 스트레인 적용
	FractureMesh->ApplyExternalStrain(
		0,
		GetActorLocation() + StrainLocationOffset, // 위치
		1000.f,              // Radius
		1,                  // PropagationDepth
		1.f,                // PropagationFactor
		10000.f              // Strain (파괴 강도)
	);

	// 파괴 이펙트
	if (DestroyEffect != nullptr)
	{
		FTransform SpawnTM = GetActorTransform();
		SpawnTM.SetScale3D(FVector(15.f));

		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			DestroyEffect,
			SpawnTM
		);
	}

	for (const FDestroyParticleInfo& Info : EndDestroyParticles)
	{
		if (Info.Particle == nullptr)
		{
			continue;
		}

		FTransform SpawnTM = Info.Transform;
		SpawnTM = SpawnTM * GetActorTransform(); // 액터 기준 위치

		FVector FinalScale = SpawnTM.GetScale3D() * Info.ScaleMultiplier;
		SpawnTM.SetScale3D(FinalScale);

		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			Info.Particle,
			SpawnTM
		);
	}

	SetLifeSpan(8.f); // 8초 후 Destroy
}