// Fill out your copyright notice in the Description page of Project Settings.


#include "ACBush.h"

#include "AnimalCrime.h"
#include "Character/ACMafiaCharacter.h"
#include "Component/ACDestroyableStatComponent.h"
#include "Component/ACShopComponent.h"
#include "Game/ACGameRuleManager.h"
#include "Game/ACMainGameMode.h"
#include "Item/ACItemData.h"

AACBush::AACBush()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// Mesh 생성 및 붙이기
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionProfileName(FName("DestroyableCollision"));
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Cartoon_City_Free/Meshes/Vegetation/SM_Bush_06.SM_Bush_06'"));
	if (MeshRef.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshRef.Object);
	}
}

void AACBush::BeginPlay()
{
	AC_LOG(LogHY, Log, TEXT("Begin"));
	Super::BeginPlay();
	
	EarnScore = 100;
	
	AC_LOG(LogHY, Log, TEXT("End"));
}

void AACBush::PostNetInit()
{
	AC_LOG(LogHY, Log, TEXT("Begin"));
	Super::PostNetInit();
	AC_LOG(LogHY, Log, TEXT("End"));
}

void AACBush::PostInitializeComponents()
{
	AC_LOG(LogHY, Log, TEXT("Begin"));
	Super::PostInitializeComponents();
	AC_LOG(LogHY, Log, TEXT("End"));
}

void AACBush::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
