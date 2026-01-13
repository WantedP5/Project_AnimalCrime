
#include "EscapeQuest/ACEscapeArea.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Character/ACMafiaCharacter.h"
#include "Game/ACMainGameState.h"
#include "Game/ACMainPlayerController.h"
#include "Game/ACPlayerState.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "AnimalCrime.h"
AACEscapeArea::AACEscapeArea()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraRef(TEXT("/Game/Project/EscapeQuest/NS_EscapeArea.NS_EscapeArea"));
	if (NiagaraRef.Succeeded())
	{
		NiagaraEffect->SetAsset(NiagaraRef.Object);
	}

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

	if (HasAuthority() == false)
	{
		AC_LOG(LogSY, Log, TEXT("HasAuthority false!"));
		return;
	}

	AACMainGameState* GS = GetWorld()->GetGameState<AACMainGameState>();
	if (GS == nullptr)
	{
		return;
	}

	AACMainPlayerController* PC = Cast<AACMainPlayerController>(Mafia->GetController());
	if (PC == nullptr)
	{
		return;
	}

	AACPlayerState* PS = PC->GetPlayerState<AACPlayerState>();
	if (PS == nullptr)
	{
		return;
	}

	// 탈출인원 추가
	GS->AddEscapedCount();

	// UI변경, IMC 변경
	PC->ClientOnEscapeSuccess();

	//관전 상태로 전환
	PS->EnterSpectatorState();

	//음성 그룹 변경
	Mafia->SetVoiceGroup(EVoiceGroup::Escape);

	//1초 뒤에 관전 실행
	FTimerHandle SpectateTimerHandle;
	FTimerDelegate SpectateDelegate;
	SpectateDelegate.BindUFunction(
		PC,
		FName("ServerStartSpectateOtherPlayer")
	);
	GetWorld()->GetTimerManager().SetTimer( 
		SpectateTimerHandle,
		SpectateDelegate,
		1.0f,
		false
	);

	//1.5초 뒤에 대기 구역으로 이동 (탈출구역에서 소리 들리는 걸 방지)
	FTimerHandle MoveTimerHandle;
	FTimerDelegate MoveDelegate;
	MoveDelegate.BindUFunction(Mafia, FName("MoveToEscapeWaitingLocation"));
	GetWorld()->GetTimerManager().SetTimer(
		MoveTimerHandle,
		MoveDelegate,
		1.5f,
		false
	);
}

