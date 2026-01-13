
#include "EscapeQuest/ACDestructibleBuilding.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

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
	//static ConstructorHelpers::FObjectFinder<UGeometryCollection> GC_Building(
	//	TEXT("/Game/Project/EscapeQuest/GC_SM_Building_Skyscraper.GC_SM_Building_Skyscraper")
	//);

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

	static ConstructorHelpers::FObjectFinder<UMaterial> BankFadeRef(
		TEXT("/Game/Project/Blueprint/DestroyObject/Material/M_BankFade.M_BankFade")
	);

	if (BankFadeRef.Succeeded())
	{
		FadeMaterial = BankFadeRef.Object;
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

void AACDestructibleBuilding::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 타이머 정리
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(FadeTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
	}

	// Strain 타이머도 정리
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(StrainTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(StrainTimerHandle);
	}

	OverlayDynamicMaterial = nullptr;

	Super::EndPlay(EndPlayReason);
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
	// Fade 타이머 중지 (파괴되면 Fade 효과 제거)
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(FadeTimerHandle))
	{
		StopFade();
	}

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
	
	const FVector Origin = GetActorLocation() + StrainLocationOffset;

#if WITH_EDITOR
	DrawDebugSphere(
		GetWorld(),
		Origin,
		1500.f,
		32,
		FColor::Red,
		false,
		5.f
	);
#endif

	// AddRadialImpulse 사용 (물리 기반)
	FractureMesh->AddRadialImpulse(
		Origin,
		1000.f,      // Radius
		5000.f,    // Strength (큰 값 필요)
		ERadialImpulseFalloff::RIF_Linear,
		true         // bVelChange
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

void AACDestructibleBuilding::StartFadeLoop()
{
	// 클라이언트에서만 실행되어야 함 (시각 효과)
	if (FadeMaterial == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("FadeMaterial is null on %s"), *GetName());
		return;
	}

	if (bDestroyed == true)
	{
		return;  // 이미 파괴된 건물은 제외
	}

	// Dynamic Material Instance 생성
	OverlayDynamicMaterial = UMaterialInstanceDynamic::Create(FadeMaterial, this);

	if (OverlayDynamicMaterial != nullptr)
	{
		// Overlay Material로 설정
		StaticMesh->SetOverlayMaterial(OverlayDynamicMaterial);

		// 초기값 설정
		CurrentFadeAlpha = 0.0f;
		bFadingIn = true;
		OverlayDynamicMaterial->SetScalarParameterValue(FName("FadeOpacity"), CurrentFadeAlpha);
	}

	// === GetWorld() 체크 추가 ===
	if (GetWorld() != nullptr)
	{
		GetWorld()->GetTimerManager().SetTimer(
			FadeTimerHandle,
			this,
			&AACDestructibleBuilding::UpdateFade,
			0.016f,
			true
		);
	}
}

void AACDestructibleBuilding::UpdateFade()
{
	// Alpha 값 업데이트
	float DeltaAlpha = FadeSpeed * 0.016f;

	if (bFadingIn == true)
	{
		CurrentFadeAlpha += DeltaAlpha;
		if (CurrentFadeAlpha >= 0.25f)
		{
			CurrentFadeAlpha = 0.25f;
			bFadingIn = false;
		}
	}
	else
	{
		CurrentFadeAlpha -= DeltaAlpha;
		if (CurrentFadeAlpha <= 0.05f)
		{
			CurrentFadeAlpha = 0.05f;
			bFadingIn = true;
		}
	}

	// Dynamic Material에 Alpha 업데이트
	if (OverlayDynamicMaterial != nullptr)
	{
		OverlayDynamicMaterial->SetScalarParameterValue(FName("FadeOpacity"), CurrentFadeAlpha);
	}
}

void AACDestructibleBuilding::StopFade()
{
	// 타이머 중지
	if (GetWorld() != nullptr)
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
	}

	// Overlay Material 제거
	if (StaticMesh != nullptr)
	{
		StaticMesh->SetOverlayMaterial(nullptr);
	}


	OverlayDynamicMaterial = nullptr;
	CurrentFadeAlpha = 0.0f;
	bFadingIn = true;
}