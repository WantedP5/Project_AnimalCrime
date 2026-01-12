
#include "EscapeQuest/ACContrabandSpawnPoint.h"
#include "Components/BillboardComponent.h"
#include "Components/SceneComponent.h"

AACContrabandSpawnPoint::AACContrabandSpawnPoint()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

#if WITH_EDITORONLY_DATA
	Billboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	if (Billboard)
	{
		Billboard->SetupAttachment(RootComponent);
	}
#endif
}
