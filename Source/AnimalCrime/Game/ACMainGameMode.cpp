

#include "ACMainGameMode.h"

#include "ACAdvancedFriendsGameInstance.h"
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
#include "ACGameEnums.h"
#include "AnimalCrime.h"

AACMainGameMode::AACMainGameMode()
{
	PlayerControllerClass = AACMainPlayerController::StaticClass();
	//DefaultPawnClass = AACMafiaCharacter::StaticClass();
	DefaultPawnClass = nullptr;

	GameStateClass = AACMainGameState::StaticClass();
	PlayerStateClass = AACPlayerState::StaticClass();

	//MafiaPawnClass = AACMafiaCharacter::StaticClass();
	static ConstructorHelpers::FClassFinder<APawn> MafiaPawnBP(
		TEXT("/Game/Project/Character/BP_ACMafia")
	);

	if (MafiaPawnBP.Succeeded())
	{
		MafiaPawnClass = MafiaPawnBP.Class;
	}
	PolicePawnClass = AACPoliceCharacter::StaticClass();

	bUseSeamlessTravel = true;
}

void AACMainGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	AC_LOG(LogHY, Warning, TEXT("Begin"));
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	AC_LOG(LogHY, Warning, TEXT("End"));
}

APlayerController* AACMainGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	AC_LOG(LogHY, Warning, TEXT("Begin"));
	APlayerController* PlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
	AC_LOG(LogHY, Warning, TEXT("End"));
	return PlayerController;
}

void AACMainGameMode::PostLogin(APlayerController* NewPlayer)
{
	AC_LOG(LogHY, Warning, TEXT("Begin"));
	Super::PostLogin(NewPlayer);
	AC_LOG(LogHY, Warning, TEXT("End"));
}

void AACMainGameMode::StartPlay()
{
	AC_LOG(LogHY, Warning, TEXT("Begin"));
	Super::StartPlay();
	AC_LOG(LogHY, Warning, TEXT("End"));
}

void AACMainGameMode::BeginPlay()
{
	AC_LOG(LogHY, Warning, TEXT("Begin"));
	Super::BeginPlay();
	AC_LOG(LogHY, Warning, TEXT("End"));

	// Game Rule Manager 생성 및 초기화
	GameRuleManager = NewObject<UACGameRuleManager>(this);
	GameRuleManager->Init(this);
	
	GenerateOutfitPool();
	SpawnAllAI();
	AC_LOG(LogHY, Warning, TEXT("End"));
}

AActor* AACMainGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	Super::ChoosePlayerStart_Implementation(Player);
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

	int32 PlayerIndex = GameState ? GameState->PlayerArray.Num() - 1 : 0;
	return PlayerStarts.Num() > 0 ? PlayerStarts[PlayerIndex % PlayerStarts.Num()] : nullptr;
}

UClass* AACMainGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController == nullptr)
	{
		return DefaultPawnClass;
	}

	AACPlayerState* PS = InController->GetPlayerState<AACPlayerState>();

	if (PS == nullptr)
	{
		return DefaultPawnClass;
	}
	UACAdvancedFriendsGameInstance* GI = GetGameInstance<UACAdvancedFriendsGameInstance>();

	if (GI == nullptr)
	{
		return DefaultPawnClass;
	}

	// GameInstance에서 역할 복원
	const FUniqueNetIdRepl& NetId = PS->GetUniqueId();

	AC_LOG(LogSY, Log, TEXT("%s : %s"), *PS->GetPlayerName(), *PS->GetUniqueId()->ToString());

	if (NetId.IsValid() == false)
	{
		return DefaultPawnClass;
	}

	if (GI->SavedPlayerRoles.Contains(NetId))
	{
		const EPlayerRole PlayerRole = GI->SavedPlayerRoles[NetId];

		PS->PlayerRole = PlayerRole;
		
		return (PlayerRole == EPlayerRole::Police) ? PolicePawnClass : MafiaPawnClass;
	}

	return DefaultPawnClass;
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

			USkeletalMesh* LoadedHair = OutfitCombo.HairAsset.LoadSynchronous();
			if (LoadedHair)
			{
				NewAI->SetHeadMesh(LoadedHair);
				NewAI->OnRep_HeadMesh();
				//NewAI->UpdateHeadMesh(LoadedHair);
				UE_LOG(LogTemp, Log, TEXT("Loaded Mesh: %s"), *LoadedHair->GetName());
			}
			
			USkeletalMesh* LoadedFace = OutfitCombo.FaceAsset.LoadSynchronous();
			if (LoadedFace)
			{
				NewAI->SetFaceMesh(LoadedFace);
				NewAI->OnRep_FaceMesh();
				UE_LOG(LogTemp, Log, TEXT("Loaded Mesh: %s"), *LoadedFace->GetName());
			}

			USkeletalMesh* LoadedTop = OutfitCombo.TopAsset.LoadSynchronous();
			if (LoadedTop)
			{
				//NewAI->TopMesh = LoadedTop;
				//NewAI->GetTopMesh()->SetTopMesh(LoadedMesh);
				NewAI->SetTopMesh(LoadedTop);
				NewAI->OnRep_TopMesh();
				UE_LOG(LogTemp, Log, TEXT("Loaded Top: %s"), *LoadedTop->GetName());
			}

			USkeletalMesh* LoadedBottom = OutfitCombo.BottomAsset.LoadSynchronous();
			if (LoadedBottom)
			{
				NewAI->SetBottomMesh(LoadedBottom);
				NewAI->OnRep_BottomMesh();
				UE_LOG(LogTemp, Log, TEXT("Loaded Bottom: %s"), *LoadedBottom->GetName());
			}
			
			USkeletalMesh* LoadedShoes = OutfitCombo.ShoesAsset.LoadSynchronous();
			if (LoadedShoes)
			{
				NewAI->SetShoesMesh(LoadedShoes);
				NewAI->OnRep_ShoesMesh();
				UE_LOG(LogTemp, Log, TEXT("Loaded Bottom: %s"), *LoadedShoes->GetName());
			}
			
			USkeletalMesh* LoadedFaceAcc = OutfitCombo.FaceAccAsset.LoadSynchronous();
            if (LoadedFaceAcc)
            {
            	NewAI->SetFaceAccMesh(LoadedFaceAcc);
            	NewAI->OnRep_FaceAccMesh();
            	UE_LOG(LogTemp, Log, TEXT("Loaded Bottom: %s"), *LoadedFaceAcc->GetName());
            }
		}

		NewAI->FinishSpawning(Transform);
		UE_LOG(LogTemp, Log, TEXT("Spawn Success"));
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
		for (TSoftObjectPtr<USkeletalMesh>& Face : FaceList)
		{
			for (TSoftObjectPtr<USkeletalMesh>& Top : TopList)
			{
				for (TSoftObjectPtr<USkeletalMesh>& Bottom : BottomList)
				{
					for (TSoftObjectPtr<USkeletalMesh>& Shoe : ShoesList)
					{
						for (TSoftObjectPtr<USkeletalMesh>& FaceAcc : FaceAccList)
						{
							FOutfitCombo Combo;
							Combo.HairAsset = Hair;
							Combo.FaceAsset = Face;
							Combo.TopAsset = Top;
							Combo.BottomAsset = Bottom;
							Combo.ShoesAsset = Shoe;
							Combo.FaceAccAsset = FaceAcc;

							OutfitPool.Add(Combo);
						}
					}
				}
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

	// // 모든 PlayerController 초기화
	// for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	// {
	// 	APlayerController* PC = It->Get();
	// 	if (PC && PC->GetPawn() == nullptr)
	// 	{
	// 		UE_LOG(LogTemp, Error, TEXT("들어오니"));
	// 		// 새로운 Pawn Spawn 후 Possess
	// 		APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, PC->GetSpawnLocation(), FRotator::ZeroRotator);
	// 		PC->Possess(NewPawn);
	//
	// 		// 클라이언트에서도 새 Pawn Possess 필요하면 RPC 호출 가능
	// 		PC->ClientRestart(NewPawn);
	// 	}
	// }
}
