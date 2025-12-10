
#include "Character/ACTestMafiaCharacter.h"
#include "Item/ACEscapeMissionBomb.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "ACTestBlackMarketDealer.h"
#include "EscapeQuest/ACBombInstallArea.h"
#include "Kismet/GameplayStatics.h"

#include "AnimalCrime.h"

void AACTestMafiaCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);  
    DOREPLIFETIME(AACTestMafiaCharacter, HandBomb);
}

void AACTestMafiaCharacter::Interact(const FInputActionValue& Value)
{
    AC_LOG(LogSY, Log, TEXT("Interact Key!!"));
	ServerInteract(); //서버에 알림.
}

void AACTestMafiaCharacter::ItemDrop(const FInputActionValue& Value)
{
    AC_LOG(LogSY, Log, TEXT("ItemDrop Key!!"));
	ServerItemDrop(); //서버에 알림.
}

void AACTestMafiaCharacter::ServerInteract_Implementation()
{
    if (InteractBomb != nullptr)
    {
        if (HandBomb != nullptr)
        {
            AC_LOG(LogSY, Warning, TEXT("Already Have Bomb"));
            return;
        }

        InteractBomb->AttachedCharacter = this;

        //아이템 핸드로 이동
        HandBomb = InteractBomb;

        //캐릭터에 폭탄 부착
        InteractBomb->AttachToCharacter();

        //폭탄 설치 가능 구역 보이게 하기
        ClientSetBombAreaVisible(true);
        AC_LOG(LogSY, Log, TEXT("Interact Bomb Success"));

        return;
    }
    if(InteractDealer != nullptr)
    {
        AC_LOG(LogSY, Log, TEXT("Interact Dealer Success"));
        InteractDealer->OnInteracted(this);
        return;
	}

    AC_LOG(LogSY, Warning, TEXT("Not Interact Actor"));
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

        //폭탄 설치 가능 구역 숨기기
        ClientSetBombAreaVisible(false);
        AC_LOG(LogSY, Log, TEXT("Bomb dropped"));
    }
    else
    {
		AC_LOG(LogSY, Warning, TEXT("No bomb to drop"));
    }
}

void AACTestMafiaCharacter::ClientSetBombAreaVisible_Implementation(bool bVisible)
{
    AC_LOG(LogSY, Log, TEXT("ClientSetBombAreaVisible1"));

    TArray<AActor*> Areas;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACBombInstallArea::StaticClass(), Areas);

    AC_LOG(LogSY, Log, TEXT("%d"), Areas.Num());
    for (AActor* Area : Areas)
    {
        AC_LOG(LogSY, Log, TEXT("ClientSetBombAreaVisible2"));
        Area->SetActorHiddenInGame(!bVisible);
    }

    AC_LOG(LogSY, Log, TEXT("ClientSetBombAreaVisible3"));
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
