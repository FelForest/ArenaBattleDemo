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
    // 아이템 타입을 가지는 데이터
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    EItemType Type;

};
