// Fill out your copyright notice in the Description page of Project Settings.


#include "StartPosition/ACPlayerStart.h"

#include "AnimalCrime.h"

void AACPlayerStart::PostInitializeComponents()
{
	AC_LOG(LogHY, Error, TEXT("Begin"));
	Super::PostInitializeComponents();
	AC_LOG(LogHY, Error, TEXT("End"));
}

void AACPlayerStart::PostNetInit()
{
	AC_LOG(LogHY, Error, TEXT("Begin"));
	Super::PostNetInit();
	AC_LOG(LogHY, Error, TEXT("End"));
}

void AACPlayerStart::BeginPlay()
{
	AC_LOG(LogHY, Error, TEXT("Begin"));
	Super::BeginPlay();
	AC_LOG(LogHY, Error, TEXT("End"));
}
