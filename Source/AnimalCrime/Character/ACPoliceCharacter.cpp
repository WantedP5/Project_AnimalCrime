// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ACPoliceCharacter.h"
#include "AnimalCrime.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

AACPoliceCharacter::AACPoliceCharacter()
{
	// Collision 세팅
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PoliceCollision"));
	
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
