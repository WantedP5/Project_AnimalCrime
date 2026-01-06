
#include "ACBillboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UACBillboardComponent::UACBillboardComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.TickInterval = 0.1f;
	SetIsReplicatedByDefault(false); // 빌보드 회전은 복제 불필요
}


void UACBillboardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC == nullptr || PC->IsLocalController() == false)
    {
        return;
    }

    APlayerCameraManager* CameraManager = PC->PlayerCameraManager;
    if (CameraManager == nullptr)
    {
        return;
    }

    FVector CameraLocation = CameraManager->GetCameraLocation();
    FVector BillboardLocation = GetComponentLocation();

    FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(BillboardLocation, CameraLocation);
    SetWorldRotation(LookAtRot);
}

