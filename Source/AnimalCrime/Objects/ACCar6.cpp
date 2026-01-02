// Fill out your copyright notice in the Description page of Project Settings.


#include "ACCar6.h"


// Sets default values
AACCar6::AACCar6()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// ConstructorHelpers::FClassFinder<AActor> MeshClassRef(TEXT("/Script/Engine.Blueprint'/Game/Project/Blueprint/DestructibleObjects/BP_Car6.BP_Car6_C'"));
	// if (MeshClassRef.Succeeded())
	// {
	// 	ActorClass = MeshClassRef.Class;
	// }
	//
	// bReplicates = true;
}

// Called when the game starts or when spawned
void AACCar6::BeginPlay()
{
	// Super::BeginPlay();
	//
	// EarnScore = 1000;
	//
	// if (ActorClass && HasAuthority())
	// {
	// 	FActorSpawnParameters Params;
	// 	Params.Owner = this;
	//
	// 	VisualActor = GetWorld()->SpawnActor<AActor>(
	// 		ActorClass,
	// 		GetActorTransform(),
	// 		Params
	// 	);
	//
	// 	VisualActor->AttachToActor(
	// 		this,
	// 		FAttachmentTransformRules::KeepWorldTransform
	// 	);
	// }
}

void AACCar6::PostNetInit()
{
	Super::PostNetInit();
}

void AACCar6::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void AACCar6::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

