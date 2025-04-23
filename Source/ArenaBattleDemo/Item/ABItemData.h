// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Weapon = 0,
    Potion,
    Scroll
};

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API UABItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    // FPrimaryAssetId 값을 반환하는 GetPrimaryAssetId 함수 오버라이드
    // 첫번째 인자 : 태그값.
    // 두번째 인자 : 이름값, 일반적으로 GetFname()을 많이 활용함
    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId("ABItemData", GetFName());
    }
	
public:
    // 아이템 타입을 가지는 데이터
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    EItemType Type;

};
