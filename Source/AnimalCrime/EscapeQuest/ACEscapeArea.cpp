
#include "EscapeQuest/ACEscapeArea.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Character/ACMafiaCharacter.h"
#include "Game/ACMainGameState.h"
#include "Game/ACMainPlayerController.h"
#include "Game/ACPlayerState.h"

#include "AnimalCrime.h"
AACEscapeArea::AACEscapeArea()
{
	SetActorEnableCollision(false);
}
void AACEscapeArea::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AACEscapeArea::OnEscapeOverlapBegin);

	AACMainGameState* GS = GetWorld()->GetGameState<AACMainGameState>();
	if (GS == nullptr)
	{
		AC_LOG(LogSY, Log, TEXT("GS is nullptr"));
		return;
	}

	AC_LOG(LogSY, Log, TEXT("BombAreas add"));
	GS->EscapeAreas.Add(this); // 클라만 저장
}

void AACEscapeArea::OnEscapeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AACMafiaCharacter* Mafia = Cast<AACMafiaCharacter>(OtherActor);
	if (Mafia == nullptr)
	{
		AC_LOG(LogSY, Log, TEXT("Mafia is nullptr!"));
		return;
	}

	AC_LOG(LogSY, Log, TEXT("Begin EscapeAreas"));

	if (HasAuthority() == false)
	{
		AC_LOG(LogSY, Log, TEXT("HasAuthority false!"));
		return;
	}
	else
	{
		AC_LOG(LogSY, Log, TEXT("HasAuthority true!"));
	}

	AACMainPlayerController* PC = Cast<AACMainPlayerController>(Mafia->GetController());
	if (PC == nullptr)
	{
		return;
	}

	// UI변경, IMC 변경
	PC->ClientOnEscapeSuccess();

	//관전으로 변경
	AACPlayerState* PS = PC->GetPlayerState<AACPlayerState>();
	if (PS == nullptr)
	{
		return;
	}

	//관전 상태로 전환
	PS->EnterSpectatorState();

	//1초 뒤에 관전 실행
	FTimerHandle TimerHandle;
	FTimerDelegate Delegate;
	Delegate.BindUFunction(
		PC,
		FName("ServerStartSpectateOtherPlayer")
	);
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		Delegate,
		1.0f,
		false
	);

}

