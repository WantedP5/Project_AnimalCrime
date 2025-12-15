
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ACTitlePlayerController.generated.h"

UCLASS()
class ANIMALCRIME_API AACTitlePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AACTitlePlayerController();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UACGameStartScreen> GameStartScreenClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UACGameStartScreen> GameStartScreen;
};
