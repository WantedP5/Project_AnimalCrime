// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ACItemData.generated.h"

 // ItemDataAsset.h
UENUM(BlueprintType)
enum class EItemType : uint8
{
    Clothing,    // SkeletalMesh - Leader Pose로 겹침
    Weapon,      // StaticMesh - 소켓에 부착
};

UENUM(BlueprintType)
enum class EClothingSlot : uint8
{
    FullBody,   // 전신
    Hat,        // 모자
    Top,        // 상의
    Bottom,     // 하의
    Shoes,      // 신발
    Accessory   // 악세사리
};

/**
 * 
 */
UCLASS()
class ANIMALCRIME_API UACItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    // 아이템 이름
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText ItemName;

    // 아이템 가격
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Price;

    // 아이템 타입
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EItemType ItemType;

    // 부착할 소켓 이름
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName AttachSocketName;

    // 미리 렌더링된 프리뷰 이미지
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<class UTexture2D> PreviewImage;

    // 의류 (스켈레탈 메시)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "ItemType == EItemType::Clothing", EditConditionHides))
    TObjectPtr <class USkeletalMesh> ClothingMesh;

    // 무기 (스태틱 메시)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "ItemType == EItemType::Weapon", EditConditionHides))
    TObjectPtr <class UStaticMesh> EquipmentMesh;
};
