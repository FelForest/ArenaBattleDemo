// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemData.h"
#include "GameData/ABCharacterStat.h"
#include "ABWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API UABWeaponItemData : public UABItemData
{
	GENERATED_BODY()

public:

	UABWeaponItemData();
	// FPrimaryAssetId 값을 반환하는 GetPrimaryAssetId 함수 오버라이드
	// 첫번째 인자 : 태그값.
	// 두번째 인자 : 이름값, 일반적으로 GetFname()을 많이 활용함
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ABItemData", GetFName());
	}

public:
	// 제공할 무기에 대한 스켈레탈 메시.
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;
	

	// 무기 아이템이 제공할 부가 스탯 데이터
	UPROPERTY(EditAnywhere, Category = Stat)
	FABCharacterStat ModifierStat;
};
