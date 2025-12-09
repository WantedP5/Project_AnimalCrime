// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ACTestPoliceCharacter.h"

AACTestPoliceCharacter::AACTestPoliceCharacter()
{
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
