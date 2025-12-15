// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACItemPreviewCapture.generated.h"

/**

    @class   AACItemPreviewCapture
    @brief   아이템 이미지를 찍기 위한 클래스
    @details 아이템이 3D메시이고 이미지도 없어서 
			 직접 에디터 월드에서 아이템을 찍어서 이미지를 만들기 위한 클래스

**/
UCLASS()
class ANIMALCRIME_API AACItemPreviewCapture : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACItemPreviewCapture();

	/**
	   @brief 렌더타겟 초기화 (에디터/런타임 모두 사용)
	   @param bForceRecreate - 기존 렌더타겟을 삭제하고 새로 생성
   **/
	UFUNCTION(BlueprintCallable, Category = "Item Preview")
	void InitializeRenderTarget(bool bForceRecreate = false);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
 /**
     @brief 아이템 데이터를 설정하고 메시 업데이트
     @param InItemData - 아이템 데이터
 **/
	UFUNCTION(BlueprintCallable, Category = "Item Preview")
	void SetItemData(class UACItemData* InItemData);

	/**
	   @brief 메시 바운드에 맞게 카메라 거리 자동 조정
   **/
	UFUNCTION(BlueprintCallable, Category = "Item Preview")
	void AutoAdjustCamera();

 /**
     @brief 위젯에 사용할 렌더타겟 가져오기
     @retval  - 렌더타겟
 **/
	UFUNCTION(BlueprintCallable, Category = "Item Preview")
	UTextureRenderTarget2D* GetRenderTarget() const { return RenderTarget; }

protected:

	// ===== 컴포넌트 =====
	// 루트 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USceneComponent> SceneRoot;

	// 3D 메시를 렌더타겟에 캡처하는 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USceneCaptureComponent2D> SceneCaptureComponent;

	// 의류 표시용(스켈레탈 메시)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USkeletalMeshComponent> ClothingMeshComponent;

	// 무기 표시용(스태틱 메시)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> EquipmentMeshComponent;
	

	// ===== 설정 =====
    // 렌더 타겟 (에디터에서 할당하거나 코드로 생성)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render Settings")
	TObjectPtr<class UTextureRenderTarget2D> RenderTarget;

	// 렌더 타겟 해상도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render Settings")
	int32 RenderTargetSize = 512;

	// 카메라 FOV
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render Settings")
	float CameraFOV = 60.0f;

	// 카메라와 메시 사이 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render Settings")
	float CameraDistance = 150.0f;

	// ===== 데이터 =====
	// 현재 표시중인 아이템
	UPROPERTY(BlueprintReadOnly, Category = "Item Preview")
	TObjectPtr<class UACItemData> CurrentItemData;
};
