

#include "ACMainGameMode.h"

#include "ACGameRuleManager.h"
#include "ACMainGameState.h"
#include "ACPlayerState.h"
#include "ACMainPlayerController.h"
#include "NavigationSystem.h"
#include "Character/ACCharacter.h"
#include "Character/ACCitizen.h"
#include "Character/ACMafiaCharacter.h"
#include "Character/ACPoliceCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

#include "AnimalCrime.h"

AACMainGameMode::AACMainGameMode()
{
	PlayerControllerClass = AACMainPlayerController::StaticClass();
	//DefaultPawnClass = AACCharacter::StaticClass();
	DefaultPawnClass = AACMafiaCharacter::StaticClass();
	GameStateClass = AACMainGameState::StaticClass();
	PlayerStateClass = AACPlayerState::StaticClass();

	MafiaPawnClass = AACMafiaCharacter::StaticClass();
	PolicePawnClass = AACPoliceCharacter::StaticClass();
	
	bUseSeamlessTravel = true;
}

void AACMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Game Rule Manager 생성 및 초기화
	GameRuleManager = NewObject<UACGameRuleManager>(this);
	GameRuleManager->Init(this);

	// GenerateOutfitPool();
	// SpawnAllAI();
}

AActor* AACMainGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	Super::ChoosePlayerStart_Implementation(Player);
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

	int32 PlayerIndex = GameState ? GameState->PlayerArray.Num() - 1 : 0;
	return PlayerStarts.Num() > 0 ? PlayerStarts[PlayerIndex % PlayerStarts.Num()] : nullptr;
}

void AACMainGameMode::HandleSeamlessTravelPlayer(AController*& Controller)
{
	Super::HandleSeamlessTravelPlayer(Controller);

	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC == nullptr)
	{
		return;
	}

	AACPlayerState* PS = PC->GetPlayerState<AACPlayerState>();
	if (PS == nullptr)
	{
		return;
	}

	//직업으로 폰 지정
	TSubclassOf<APawn> PawnClassToUse = nullptr;
	switch (PS->CharacterType)
	{
	case EACCharacterType::Police:
		PawnClassToUse = PolicePawnClass;
		AC_LOG(LogSY, Log, TEXT("Police!"));
		break;
	case EACCharacterType::Mafia:
		PawnClassToUse = MafiaPawnClass;
		AC_LOG(LogSY, Log, TEXT("Mafia!"));
		break;

	default:
		AC_LOG(LogSY, Log, TEXT("Default!"));
		PawnClassToUse = DefaultPawnClass;
		break;
	}
	if (PawnClassToUse == nullptr)
	{
		return;
	}

	// 기존 폰 제거
	if (APawn* OldPawn = PC->GetPawn())
	{
		OldPawn->Destroy();
	}


	//플레이어 스타트 찾기
	AActor* StartSpot = FindPlayerStart(PC);
	if (StartSpot == nullptr)
	{
		return;
	}

	// Pawn 스폰
	FActorSpawnParameters Params;
	Params.Owner = PC;
	Params.Instigator = nullptr;

	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(
		PawnClassToUse,
		StartSpot->GetActorTransform(),
		Params
	);

	if (NewPawn == nullptr)
	{
		return;
	}
	// 컨트롤러에 빙의
	PC->Possess(NewPawn);

	//클라이언트 초기화
	PC->ClientRestart(NewPawn);
}

void AACMainGameMode::AddTeamScore(int32 Score)
{
	UE_LOG(LogTemp, Log, TEXT("[AddTeamScore]"));
	AACMainGameState* MainGameState = GetGameState<AACMainGameState>();
	if (MainGameState)
	{
		MainGameState->UpdateTeamScore(MainGameState->GetTeamScore() + Score);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AddTeamScore Nullptr"));
	}
}

int32 AACMainGameMode::GetTeamScore() const
{
	UE_LOG(LogTemp, Log, TEXT("[GetTeamScore]"));
	AACMainGameState* MainGameState = GetGameState<AACMainGameState>();
	if (MainGameState)
	{
		return MainGameState->GetTeamScore();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("GetTeamScore Nullptr"));
	}
	return -1;
}

void AACMainGameMode::UpdateGameScoreFromPolice(EPoliceAction PoliceActionType, float Score)
{
	GameRuleManager->RewardPoliceForAction(PoliceActionType, Score);
}

void AACMainGameMode::UpdateGameScoreFromMafia(EMafiaAction MafiaActionType, float Score)
{
	GameRuleManager->RewardMafiaForAction(MafiaActionType, Score);
}

void AACMainGameMode::UpdateGameScoreFromObject(float Score)
{
	// @Todo: 나중에 구현
}

float AACMainGameMode::GetGameScore() const
{
	UE_LOG(LogTemp, Log, TEXT("[GetGameScore]에서 GameRuleManager->GetScoreGauge을 호출합니다."));

	return GameRuleManager->GetScoreGauge();
}

void AACMainGameMode::SpawnAllAI()
{
	for (int32 i = 0; i < AINums; ++i)
	{
		FVector Pos = GetRandomSpawnLocation();
		FRotator Rot = FRotator::ZeroRotator;

		FTransform Transform(Pos);
		Transform.SetRotation(Rot.Quaternion());
		AACCitizen* NewAI = GetWorld()->SpawnActorDeferred<AACCitizen>(AACCitizen::StaticClass(), Transform);
		if (NewAI)
		{
			// AI 관리하는 애
			AIObjectArray.Add(NewAI);

			FOutfitCombo OutfitCombo = GiveOutfitFromPool();

			USkeletalMesh* LoadedMesh = OutfitCombo.HairAsset.LoadSynchronous();
			if (LoadedMesh)
			{
				NewAI->HeadMesh->SetSkeletalMesh(LoadedMesh);
				UE_LOG(LogTemp, Log, TEXT("Loaded Mesh: %s"), *LoadedMesh->GetName());
			}

			USkeletalMesh* LoadedTop = OutfitCombo.TopAsset.LoadSynchronous();
			if (LoadedTop)
			{
				NewAI->TopMesh->SetSkeletalMesh(LoadedTop);
				UE_LOG(LogTemp, Log, TEXT("Loaded Top: %s"), *LoadedTop->GetName());
			}

			USkeletalMesh* LoadedBottom = OutfitCombo.BottomAsset.LoadSynchronous();
			if (LoadedBottom)
			{
				NewAI->BottomMesh->SetSkeletalMesh(LoadedBottom);
				UE_LOG(LogTemp, Log, TEXT("Loaded Bottom: %s"), *LoadedBottom->GetName());
			}
		}

		NewAI->FinishSpawning(Transform);

	}

}

FVector AACMainGameMode::GetRandomSpawnLocation() const
{
	FNavLocation RandomLocation;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	float Radius = 3000;
	if (NavSys && NavSys->GetRandomReachablePointInRadius(FVector::ZeroVector, Radius, RandomLocation))
	{
		return RandomLocation.Location;
	}

	return FVector::ZeroVector;
}

void AACMainGameMode::GenerateOutfitPool()
{
	OutfitPool.Empty();

	for (TSoftObjectPtr<USkeletalMesh>& Hair : HairList)
	{
		for (TSoftObjectPtr<USkeletalMesh>& Top : TopList)
		{
			for (TSoftObjectPtr<USkeletalMesh>& Bottom : BottomList)
			{
				FOutfitCombo Combo;
				Combo.HairAsset = Hair;
				Combo.TopAsset = Top;
				Combo.BottomAsset = Bottom;

				OutfitPool.Add(Combo);
			}
		}
	}

	// 랜덤 셔플
	OutfitPool.Sort([](auto&, auto&) { return FMath::RandBool(); });

	NextOutfitIndex = 0;
}

FOutfitCombo AACMainGameMode::GiveOutfitFromPool()
{
	// int32 CurrentOutfitIndex = NextOutfitIndex++;
	// return OutfitPool[CurrentOutfitIndex];

	if (OutfitPool.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("OutfitPool is EMPTY!"));
		return FOutfitCombo();
	}

	if (NextOutfitIndex >= OutfitPool.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("OutfitPool INDEX OUT OF RANGE: %d / %d"),
			NextOutfitIndex, OutfitPool.Num());
		return FOutfitCombo();
	}

	int32 CurrentOutfitIndex = NextOutfitIndex++;

	UE_LOG(LogTemp, Warning, TEXT("GiveOutfitFromPool: %d / %d"),
		CurrentOutfitIndex, OutfitPool.Num());

	return OutfitPool[CurrentOutfitIndex];
}

void AACMainGameMode::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();

	UE_LOG(LogTemp, Warning, TEXT("Seamless Travel 완료, 서버 PostSeamlessTravel 호출"));

	// 모든 PlayerController 초기화
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn() == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("들어오니"));
			// 새로운 Pawn Spawn 후 Possess
			APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, PC->GetSpawnLocation(), FRotator::ZeroRotator);
			PC->Possess(NewPawn);

			// 클라이언트에서도 새 Pawn Possess 필요하면 RPC 호출 가능
			PC->ClientRestart(NewPawn);
		}
	}
}
