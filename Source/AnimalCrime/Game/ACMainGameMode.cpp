

#include "ACMainGameMode.h"

#include "ACMainGameState.h"
#include "ACMainPlayerController.h"
#include "NavigationSystem.h"
#include "Character/ACCharacter.h"
#include "Character/ACCitizen.h"
#include "Character/ACTestMafiaCharacter.h"

AACMainGameMode::AACMainGameMode()
{
	PlayerControllerClass = AACMainPlayerController::StaticClass();
	//DefaultPawnClass = AACCharacter::StaticClass();
	DefaultPawnClass = AACTestMafiaCharacter::StaticClass();
	GameStateClass = AACMainGameState::StaticClass();
}

void AACMainGameMode::BeginPlay()
{
	Super::BeginPlay();
	GenerateOutfitPool();
	SpawnAllAI();
}

void AACMainGameMode::AddTeamScore(int32 Score)
{
	UE_LOG(LogTemp, Log, TEXT("[AddTeamScore]"));
	AACMainGameState* MainGameState = GetGameState<AACMainGameState>();
	if (MainGameState)
	{
		MainGameState->TeamScore += Score;  // Replicate → 모든 클라 업데이트됨
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT(" AddTeamScore Nullptr"));
	}
}

int32 AACMainGameMode::GetTeamScore() const
{
	UE_LOG(LogTemp, Log, TEXT("[GetTeamScore]"));
	AACMainGameState* MainGameState = GetGameState<AACMainGameState>();
	if (MainGameState)
	{
		return MainGameState->TeamScore;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT(" GetTeamScore Nullptr"));
	}
	return -1;
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
	if (NavSys && NavSys->GetRandomReachablePointInRadius(FVector::ZeroVector,Radius , RandomLocation))
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
