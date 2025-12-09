
#include "Character/ACTestMafiaCharacter.h"
#include "Item/ACEscapeMissionBomb.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "AnimalCrime.h"

void AACTestMafiaCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);  
    DOREPLIFETIME(AACTestMafiaCharacter, HandBomb);
}

void AACTestMafiaCharacter::Interact(const FInputActionValue& Value)
{
    AC_LOG(LogSY, Log, TEXT("Interact!!"));
	ServerInteract(); //서버에 알림.
}

void AACTestMafiaCharacter::ItemDrop(const FInputActionValue& Value)
{
    AC_LOG(LogSY, Log, TEXT("ItemDrop!!"));
	ServerItemDrop(); //서버에 알림.
}

void AACTestMafiaCharacter::ServerInteract_Implementation()
{
    if (InteractBomb == nullptr)
    {
        AC_LOG(LogSY, Warning, TEXT("Not Interact Bomb"));
        return;
    }
    InteractBomb->AttachedCharacter = this;
    
    //아이템 핸드로 이동
    HandBomb = InteractBomb;

	//캐릭터에 폭탄 부착
    InteractBomb->AttachToCharacter();

    AC_LOG(LogSY, Log, TEXT("Interact Bomb Success"));
}

void AACTestMafiaCharacter::ServerItemDrop_Implementation()
{
    if (HandBomb != nullptr)
    {
        //참조 해제
        HandBomb->AttachedCharacter = nullptr;

		//캐릭터에서 폭탄 분리
		HandBomb->DetachFromCharacter();

        // Impulse 적용은 서버가 직접 해야 하므로 여기서 처리
        //UStaticMeshComponent* MeshComp = HandBomb->GetBombMeshComp();
        //if (MeshComp)
        //{
        //    FVector ThrowDir = GetActorForwardVector() + FVector(0, 0, 0.3f);
        //    MeshComp->AddImpulse(ThrowDir * 300.f, NAME_None, true);
        //}

        HandBomb = nullptr;

        AC_LOG(LogSY, Log, TEXT("ServerDropBomb: Bomb dropped"));
    }
    else
    {
		AC_LOG(LogSY, Warning, TEXT("ServerDropBomb: No bomb to drop"));
    }
}

void AACTestMafiaCharacter::OnRep_HandBomb()
{
    if (HandBomb)
    {
        AC_LOG(LogSY, Log, TEXT("OnRep_HandBomb: Now holding bomb"));
    }
    else
    {
        AC_LOG(LogSY, Log, TEXT("OnRep_HandBomb: No longer holding bomb"));
    }
}
