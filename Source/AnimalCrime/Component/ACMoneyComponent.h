// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Objects/MoneyData.h"
#include "ACMoneyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChangedDelegate, int32, NewMoney);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ANIMALCRIME_API UACMoneyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UACMoneyComponent();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// 미사용
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	FORCEINLINE int32	GetMoney()	const			{	return  MoneyData.Money;}

	// EarnMoney(), SpendMoney(), LoseMoney() 로 사용하면 됨 
	// 확인하고 확인한 사람이 지워주세요
	/*FORCEINLINE void	UpdateMoney(int32 InMoney)	{	MoneyData.Money = InMoney;}
	FORCEINLINE void	AddMoney(int32 InMoney)		{	UpdateMoney(GetMoney() + InMoney);}
	FORCEINLINE void	SubMoney(int32 InMoney)		{	UpdateMoney(GetMoney() - InMoney);}*/
	
public:
	void InitMoneyComponent(EMoneyType MoneyType);
	
	void EarnMoney(int32 InMoney);
	bool SpendMoney(int32 InMoney);
	int32 LoseMoney(int32 InMoney);

	// 돈이 변경될 때마다 브로드캐스트
	UPROPERTY(BlueprintAssignable, Category = "Money")
	FOnMoneyChangedDelegate OnMoneyChanged;

	// OnRep 함수 추가
	UFUNCTION()
	void OnRep_MoneyData();
	
public:
	void GenerateRandomMoney(int32 InMaxMoney);
	
private:
	// 1. Mafia 세팅
	void InitMafiaSetting();
	
	// 2. Police 세팅
	void InitPoliceSetting();
	
	// 3. Citizen 세팅
	void InitCitizenSetting();
	
	void InitMoney(int32 InMoney);
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_MoneyData, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FMoneyData MoneyData;
};
