// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ACPoliceCharacter.h"
#include "AnimalCrime.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Component/ACMoneyComponent.h"

AACPoliceCharacter::AACPoliceCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HeadMeshRef(TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Hat_057.SK_Hat_057"));
	if (HeadMeshRef.Succeeded() == true)
	{
		HeadMesh->SetSkeletalMesh(HeadMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FaceAccMeshRef(TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Moustache_002.SK_Moustache_002"));
	if (FaceAccMeshRef.Succeeded() == true)
	{
		FaceAccMesh->SetSkeletalMesh(FaceAccMeshRef.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FaceMeshRef(TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Male_emotion_angry_003.SK_Male_emotion_angry_003"));
	if (FaceMeshRef.Succeeded() == true)
	{
		FaceMesh->SetSkeletalMesh(FaceMeshRef.Object);
	}

	TopMesh->SetSkeletalMesh(nullptr);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BottomMeshRef(TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Costume_10_001.SK_Costume_10_001"));
	if (BottomMeshRef.Succeeded() == true)
	{
		BottomMesh->SetSkeletalMesh(BottomMeshRef.Object);
	}

	ShoesMesh->SetSkeletalMesh(nullptr);
}

EACCharacterType AACPoliceCharacter::GetCharacterType()
{
	return EACCharacterType::Police;
}

void AACPoliceCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Police는 200원으로 시작
	if (HasAuthority())
	{
		MoneyComp->InitMoneyComponent(EMoneyType::MoneyPoliceType);
		UE_LOG(LogHG, Log, TEXT("Police BeginPlay: MoneyPoliceType 초기화"));
	}
}

bool AACPoliceCharacter::CanInteract(AACCharacter* ACPlayer)
{
	// 경찰끼리도 소통 가능?
	return true;
}

void AACPoliceCharacter::OnInteract(AACCharacter* ACPlayer)
{
	ShowInteractDebug(ACPlayer);
}

FString AACPoliceCharacter::GetInteractableName() const
{
	return TEXT("Police");
}
