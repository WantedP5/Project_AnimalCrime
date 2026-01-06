// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ACInteractInterface.h"
#include "ACCCTVArea.generated.h"

/**

    @class   AACCCTVArea
    @brief   CCTV 구역에서 인터랙트를 하게 해주는 클래스
    @details ACInteractInterface를 상속받아 오버라이드로 구현

**/
UCLASS()
class ANIMALCRIME_API AACCCTVArea : public AActor, public IACInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACCCTVArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
		@brief 실제 상호작용 로직(서버에서 실행)
		@param ACPlayer - 타입 체크를 위한 캐릭터 가져오기
	**/
	virtual void OnInteract(class AACCharacter* ACPlayer, EInteractionKey InKey) override;

	virtual EACInteractorType GetInteractorType() const override;

	//=== 위젯 인터페이스 ===
	virtual class UWidgetComponent* GetInteractionWidget() const override;
	virtual void ShowInteractionHints(const TArray<class UACInteractionData*>& Interactions) override;
	virtual void HideInteractionHints() override;

	/**
	  @brief 콜리전 범위를 벗어났을 때 CCTV UI 자동으로 닫기
	  @param OverlappedComponent - InteractBoxComponent
	  @param OtherActor - 콜리전을 벗어난 액터 (플레이어)
	  @param OtherComp - 벗어난 액터의 컴포넌트
	  @param OtherBodyIndex - 바디 인덱스
  **/
	UFUNCTION()
	void OnInteractBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Scene Capture 활성화/비활성화 함수
	UFUNCTION(BlueprintCallable, Category = "CCTV")
	void SetSceneCaptureActive(bool bActive);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TObjectPtr<class UACInteractableComponent> InteractBoxComponent;

	//!< 상호작용 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact")
	TObjectPtr<class UWidgetComponent> InteractionWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TSubclassOf<class UACInteractionInfoWidget> InteractionInfoWidgetClass;

	// 상점 UI 위젯 클래스(블루프린트에서 설정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TSubclassOf<class UACCCTVWidget> CCTVWidgetClass;

	// CCTV Scene Capture 컴포넌트들 (블루프린트에서 설정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CCTV")
	TArray<class USceneCaptureComponent2D*> SceneCaptureComponents;
};
