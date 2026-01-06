// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
// todo: 액터 전용 로그라서 그 외 클래스에서는 사용 불가

// Local Role.
#define LOG_SUBLOCALROLEINFO \
    *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwner()->GetLocalRole()))

// Remote Role.
#define LOG_SUBREMOTEROLEINFO \
    *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwner()->GetRemoteRole()))

// Local Role.
#define LOG_LOCALROLEINFO \
    *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetLocalRole()))

// Remote Role.
#define LOG_REMOTEROLEINFO \
    *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole()))

// NetMode.
#define LOG_NETMODEINFO ( (GetNetMode() == ENetMode::NM_Client) ? \
    *FString::Printf(TEXT("Client_%d"), UE::GetPlayInEditorID()) : \
    (GetNetMode() == ENetMode::NM_Standalone) ? \
    TEXT("StandAlone") : TEXT("Server") )

// __FUNCTION__ -> const char* (ANSI).
#define LOG_CALLINFO ANSI_TO_TCHAR(__FUNCTION__)

#define AC_LOG(LogCat, Verbosity, Format, ...) \
UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s %s"), \
    LOG_NETMODEINFO, LOG_LOCALROLEINFO, LOG_REMOTEROLEINFO, LOG_CALLINFO, \
    *FString::Printf(Format, ##__VA_ARGS__))

#define AC_SUBLOG(LogCat, Verbosity, Format, ...) \
UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s %s"), \
    LOG_NETMODEINFO, LOG_SUBLOCALROLEINFO, LOG_SUBREMOTEROLEINFO, LOG_CALLINFO, \
    *FString::Printf(Format, ##__VA_ARGS__))

DECLARE_LOG_CATEGORY_EXTERN(LogSW, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSY, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogHG, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogHY, Log, All);

#define DESTROYABLE_COL     TEXT("DestroyableCollision") 
#define INTERACTABLE_COL    TEXT("InteractableCollision")
#define PEOPLE_COL          TEXT("PeopleCollision")
#define MAFIA_COL           TEXT("MafiaCollision") 
#define POLICE_COL          TEXT("PoliceCollision")
#define CITIZEN_COL         TEXT("CitizenCollision")


#define BREAK               ECC_GameTraceChannel2
#define ATTACK_TARGET       ECC_GameTraceChannel4

#define DESTROYABLE_OBJ     ECC_GameTraceChannel1
#define INTERACTABLE_OBJ    ECC_GameTraceChannel3
#define PEOPLE_OBJ          ECC_GameTraceChannel5
#define MAFIA_OBJ           ECC_GameTraceChannel6
#define POLICE_OBJ          ECC_GameTraceChannel7
#define CITIZEN_OBJ         ECC_GameTraceChannel8
