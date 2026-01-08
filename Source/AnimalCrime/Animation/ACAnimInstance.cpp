// Fill out your copyright notice in the Description page of Project Settings.


#include "ACAnimInstance.h"

#include "AnimalCrime.h"
#include "KismetAnimationLibrary.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UACAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Character = Cast<ACharacter>(TryGetPawnOwner());
	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
	}
}

void UACAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Character || !MovementComponent)
	{
		return;
	}

	/* Velocity */
	Velocity = MovementComponent->Velocity;

	/* Ground Speed (XY only) */
	GroundSpeed = FVector2D(Velocity.X, Velocity.Y).Size(); 
	UE_LOG(LogHY, Error, TEXT("ground:%f"), GroundSpeed);
	
	/* ================= Direction ================= */
	const FRotator ActorRotation = Character->GetActorRotation();

	float RawDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, ActorRotation);

	// BP에서 하던 Clamp + Select 그대로
	if (!MovementComponent->bOrientRotationToMovement)
	{
		RawDirection = FMath::Clamp(RawDirection, -45.f, 45.f);
	}

	Direction = RawDirection;
	
	/* Is Falling */
	bIsFalling = MovementComponent->IsFalling();
	
	/* Should Move */
	const FVector Acceleration = MovementComponent->GetCurrentAcceleration();
	
	const bool bHasAcceleration = (GroundSpeed > 0.01f) && (Acceleration.Size() != 0) && (Acceleration != FVector::ZeroVector);

	UE_LOG(LogHY, Error, TEXT("Acc:%f %d"), Acceleration.Size(), bShouldMove);
	bShouldMove = bHasAcceleration;

	/* Cache rotation for thread-safe use */
	bIsFalling = MovementComponent->IsFalling();
	
}

void UACAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}
