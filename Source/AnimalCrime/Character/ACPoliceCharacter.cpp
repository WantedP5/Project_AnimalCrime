// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ACPoliceCharacter.h"
#include "AnimalCrime.h"
#include "Components/SkeletalMeshComponent.h"

AACPoliceCharacter::AACPoliceCharacter()
{
	HeadMesh->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Hat_057.SK_Hat_057")));
	TopMesh->SetSkeletalMesh(nullptr);
	BottomMesh->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Costume_10_001.SK_Costume_10_001")));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FaceMeshRef(TEXT("/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Male_emotion_angry_003.SK_Male_emotion_angry_003"));
	if (FaceMeshRef.Succeeded() == true)
	{
		FaceMesh->SetSkeletalMesh(FaceMeshRef.Object);
	}
	

	// Weapon Component
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	//todo: Load Weapon through Inventory
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(
		TEXT("/Game/Weapons_Free/Meshes/SKM_baseball_bat_001.SKM_baseball_bat_001"));
	if (WeaponMeshRef.Succeeded())
	{
		WeaponMesh->SetSkeletalMesh(WeaponMeshRef.Object);
	}
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
}

EACCharacterType AACPoliceCharacter::GetCharacterType()
{
	return EACCharacterType::Police;
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
