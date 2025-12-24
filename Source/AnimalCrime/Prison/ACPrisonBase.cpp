// Fill out your copyright notice in the Description page of Project Settings.


#include "Prison/ACPrisonBase.h"
#include "Character/ACCharacter.h"
#include "Component/ACInteractableComponent.h"
#include "Net/UnrealNetwork.h"
#include "AnimalCrime.h"
#include "Components/BoxComponent.h"
#include "Game/ACMainGameMode.h"

AACPrisonBase::AACPrisonBase()
{
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	InteractBoxComponent = CreateDefaultSubobject<UACInteractableComponent>(TEXT("InteractBoxComponent"));
	InteractBoxComponent->SetupAttachment(RootComponent);
	// todo: 정확한 좌표 모르겠음.
	InteractBoxComponent->SetOffset(FVector(180, 0.f, 30.f));

	// 감옥 내부 영역 박스 생성
	PrisonAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PrisonAreaBox"));
	PrisonAreaBox->SetupAttachment(RootComponent);
	//PrisonAreaBox->SetBoxExtent(FVector(200.f, 200.f, 100.f));  // 조절 필요
	//PrisonAreaBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// todo: 임시 좌표
	TeleportOffset = FVector(0.f, 0.f, 50.f);
}

void AACPrisonBase::BeginPlay()
{
	Super::BeginPlay();
	AC_LOG(LogSW, Log, TEXT("RegisterPrison"))

	// 서버에서 GameMode의 PrisonManager에 등록
	if (HasAuthority() == true)
	{
		AACMainGameMode* GM = GetWorld()->GetAuthGameMode<AACMainGameMode>();
		AC_LOG(LogSW, Log, TEXT("RRegisterPrison"))
		ensureAlways(GM != nullptr);
		
		GM->RegisterPrison(this);  // GameMode에 캡슐화 함수 사용
	}


	// PrisonAreaBox 오버랩 이벤트 바인딩
	ensureAlways(PrisonAreaBox != nullptr);

	//PrisonAreaBox->OnComponentBeginOverlap.AddDynamic(this, &AACPrisonBase::OnPrisonAreaOverlapBegin);
	PrisonAreaBox->OnComponentEndOverlap.AddDynamic(this, &AACPrisonBase::OnPrisonAreaOverlapEnd);
	
}

void AACPrisonBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AACPrisonBase, bDoorOpen);
	DOREPLIFETIME(AACPrisonBase, Prisoners);
}

void AACPrisonBase::OnRep_DoorOpen()
{
	// 클라이언트에서 문 상태 변경 이벤트 호출
	BP_OnDoorStateChanged(bDoorOpen);
}

bool AACPrisonBase::CanInteract(AACCharacter* ACPlayer)
{
	if (ACPlayer == nullptr)
	{
		return false;
	}

	// 문이 이미 열려있으면 상호작용 불가
	if (bDoorOpen == true)
	{
		return false;
	}

	if (ACPlayer->GetCharacterType() == EACCharacterType::Mafia)
	{
		return true;
	}

	// todo: 추후에 문 수리 로직
	else if (ACPlayer->GetCharacterType() == EACCharacterType::Police)
	{
		return false;
	}

	return false;
}

void AACPrisonBase::OnInteract(AACCharacter* ACPlayer)
{
	//ShowInteractDebug(ACPlayer, GetName());

	// 서버에서만 문 열기
	if (HasAuthority() == true)
	{
		OpenDoor();
	}
}

void AACPrisonBase::OpenDoor()
{
	if (bDoorOpen == true)
	{
		return;
	}

	AC_LOG(LogSW, Log, TEXT("Prison Door Opened"));

	// 자동 레플리케이션 -> OnRep 호출
	bDoorOpen = true;

	// 서버는 직접 이벤트 호출 (OnRep은 클라이언트에서만 호출됨)
	BP_OnDoorStateChanged(true);

	// 자동 닫힘 타이머 설정
	GetWorldTimerManager().SetTimer(DoorTimerHandle, this, &AACPrisonBase::CloseDoor, AutoCloseDelay, false);
}

void AACPrisonBase::CloseDoor()
{
	if (bDoorOpen == false)
	{
		return;
	}

	AC_LOG(LogSW, Log, TEXT("Prison Door Closed"));

	// 자동 레플리케이션 -> OnRep 호출
	bDoorOpen = false;

	// 문 닫힐 때 감옥 내의 수감자 파악해서 배열에 추가.
	if (HasAuthority() == true)
	{
		UpdatePrisoners();
	}

	// 서버는 직접 이벤트 호출 (OnRep은 클라이언트에서만 호출됨)
	BP_OnDoorStateChanged(false);

	// 타이머 클리어
	GetWorldTimerManager().ClearTimer(DoorTimerHandle);
}

// todo: 추후에 경찰은 문의 피통 한칸당 5초로 수리?
float AACPrisonBase::GetRequiredHoldTime() const
{
	return 10.0f;
}

bool AACPrisonBase::IsEmpty() const
{
	return Prisoners.IsEmpty();
}

FVector AACPrisonBase::GetTeleportLocation() const
{
	return GetActorLocation() + TeleportOffset;
}

void AACPrisonBase::Imprison(AACCharacter* Character, bool bForced)
{
	if (Character == nullptr)
	{
		return;
	}

	// 이미 수감되어 있으면 무시
	//if (Prisoners.Contains(Character))
	//{
	//	return;
	//}

	// 강제 투옥이면 텔레포트
	if (bForced == true)
	{
		Character->SetActorLocation(GetTeleportLocation());
	}

	// 수감자 목록에 추가
	Prisoners.Add(Character);

	AC_LOG(LogSW, Warning, TEXT("Character %s imprisoned"), *Character->GetName());
	// todo: 임시 로그
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("%s 투옥!"), *Character->GetName()));
}

void AACPrisonBase::Release(AACCharacter* Character)
{
	if (Character == nullptr)
	{
		return;
	}

	Prisoners.Remove(Character);
	AC_LOG(LogSW, Warning, TEXT("Character %s released"), *Character->GetName());
	// todo: 임시 로그
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("%s 탈옥!"), *Character->GetName()));
}

void AACPrisonBase::UpdatePrisoners()
{
	if (PrisonAreaBox == nullptr)
	{
		return;
	}

	// 현재 PrisonAreaBox 내에 있는 모든 액터 확인
	TArray<AActor*> OverlappingActors;
	PrisonAreaBox->GetOverlappingActors(OverlappingActors, AACCharacter::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		AACCharacter* Character = Cast<AACCharacter>(Actor);
		if (Character == nullptr)
		{
			continue;
		}
		
		// 이미 수감자는 처리 안해줌
		if (Prisoners.Contains(Character) == true)
		{
			continue;
		}

		// 마피아만 수감 (경찰은 제외)
		if (Character->GetCharacterType() == EACCharacterType::Mafia)
		{
			Imprison(Character, false);
		}

	}
}

//void AACPrisonBase::OnPrisonAreaOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	// 현재는 로깅만 (문 닫힐 때 UpdatePrisoners에서 처리)
//	AACCharacter* Character = Cast<AACCharacter>(OtherActor);
//	AC_LOG(LogSW, Warning, TEXT("Character %s Overlap started"), *Character->GetName());
//}

void AACPrisonBase::OnPrisonAreaOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 탈옥 처리: 문이 열려있고 영역을 벗어나면 해방

	//if (bDoorOpen == false)
	//{
	//	return;
	//}

	AACCharacter* Character = Cast<AACCharacter>(OtherActor);
	if (Character == nullptr)
	{
		return;
	}
	AC_LOG(LogSW, Warning, TEXT("Character %s Overlap ended"), *Character->GetName());

	// 수감자였다면 풀어준다.
	if (Prisoners.Contains(Character) == true)
	{
		Release(Character);
	}

}

