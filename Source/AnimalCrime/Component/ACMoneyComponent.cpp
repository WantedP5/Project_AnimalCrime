// Fill out your copyright notice in the Description page of Project Settings.


#include "ACMoneyComponent.h"

#include "Net/UnrealNetwork.h"


UACMoneyComponent::UACMoneyComponent()
{
}



void UACMoneyComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UACMoneyComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UACMoneyComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UACMoneyComponent, MoneyData);
}

void UACMoneyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UACMoneyComponent::InitMoneyComponent(EMoneyType MoneyType)
{
	if (MoneyType == EMoneyType::None)
	{
		return ;
	}
	
	if (MoneyData.MoneyType != EMoneyType::None)
	{
		return ;
	}
	
	EMoneyType CurrentMoneyData = MoneyType;
	UE_LOG(LogTemp, Error, TEXT("여긴 오니? %d"), CurrentMoneyData);
	switch (CurrentMoneyData)
	{
	case EMoneyType::MoneyMafiaType:
		InitMafiaSetting();
		break;
	case EMoneyType::MoneyPoliceType:
		InitPoliceSetting();
		break;
	case EMoneyType::MoneyCitizenType:
		InitCitizenSetting();
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("말이 안됌."));
	}
}

void UACMoneyComponent::EarnMoney(int32 InMoney)
{
	if (InMoney < 0)
	{
		return;
	}
	
	if (MoneyData.MoneyType == EMoneyType::None)
	{
		return;
	}
	
	// 돈 획득
	MoneyData.Money += InMoney;
}

bool UACMoneyComponent::SpendMoney(int32 InMoney)
{
	if (MoneyData.MoneyType == EMoneyType::None || MoneyData.MoneyType == EMoneyType::MoneyCitizenType)
	{
		return false;
	}
	if (InMoney < 0)
	{
		return false;
	}
	if (MoneyData.Money < InMoney)
	{
		return false;
	}
	
	// 돈 소비
	MoneyData.Money -= InMoney;
	
	return true;
}

int32 UACMoneyComponent::LoseMoney(int32 InMoney)
{
	UE_LOG(LogTemp, Log, TEXT("[LoseMoney(int32 InMoney)]:%d vs %d"), InMoney, MoneyData.Money);
	if (InMoney < 0)
	{
		return -1;
	}
		
	// 크다고 리턴하면 돈이 남음.
	if (MoneyData.Money < InMoney)
	{
		InMoney = MoneyData.Money;
	}
	
	// 실제 값 계산
	MoneyData.Money = FMath::Clamp(MoneyData.Money - InMoney, 0, MoneyData.Money);
	return InMoney;
}

void UACMoneyComponent::InitMafiaSetting()
{
	MoneyData.MoneyType = EMoneyType::MoneyMafiaType;
	
	InitMoney(100);
}

void UACMoneyComponent::InitPoliceSetting()
{
	MoneyData.MoneyType = EMoneyType::MoneyPoliceType;
	
	InitMoney(200);
}

void UACMoneyComponent::InitCitizenSetting()
{
	MoneyData.MoneyType = EMoneyType::MoneyCitizenType;
	
	GenerateRandomMoney(500);
}

void UACMoneyComponent::InitMoney(int32 InMoney)
{
	MoneyData.Money = InMoney;
}

void UACMoneyComponent::GenerateRandomMoney(int32 InMaxMoney)
{
	MoneyData.Money = FMath::RandRange(0, InMaxMoney);
	UE_LOG(LogTemp, Error, TEXT("내 돈은 얼마일까? %d"), MoneyData.Money);
}



