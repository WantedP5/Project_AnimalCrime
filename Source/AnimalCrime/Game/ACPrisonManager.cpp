// Fill out your copyright notice in the Description page of Project Settings.


#include "ACPrisonManager.h"
#include "ACMainGameMode.h"
#include "Character/ACCharacter.h"
#include "Prison/ACPrisonBase.h"
#include "AnimalCrime.h"

void UACPrisonManager::RegisterPrison(AACPrisonBase* InPrison)
{
    UE_LOG(LogSW, Log, TEXT("RegisterED"))
	Prisons.Add(InPrison);
}

//void UACPrisonManager::UnregisterPrison()
//{
//}

void UACPrisonManager::ImprisonCharacter(AACCharacter* ACPlayer)
{
    AACPrisonBase* EmptyPrison = FindEmptyPrison();
    if (EmptyPrison == nullptr)
    {
        UE_LOG(LogSW, Log, TEXT("ALL PRISON ACCUPIED"));
        return;
    }

    //UE_LOG(LogSW, Log, TEXT("ALL PRISON ACCUPIED"));
    EmptyPrison->Imprison(ACPlayer, true);  // 강제 텔레포트
}

AACPrisonBase* UACPrisonManager::FindEmptyPrison()
{
    if (Prisons.Num() == 0)
    {
        UE_LOG(LogSW, Log, TEXT("NO PRISON REGISTERED"));
        return nullptr;
    }
    for (AACPrisonBase* Prison : Prisons)
    {
        if (Prison == nullptr)
        {
            continue;
        }

        // 비어있는 감옥
        if (Prison->IsEmpty() == true)
        {
            return Prison;
        }
    }

    return nullptr;
}