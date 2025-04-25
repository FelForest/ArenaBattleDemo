// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABCharacterStat.h"
#include "ABGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogABGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API UABGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UABGameSingleton();

	// 싱글톤 접근 Get
	static UABGameSingleton& Get();

	// Character Stat Data Section
public:

	// 레벨을 인덱스로 사용해 캐릭터 스탯을 가져올 수 있는 Getter
	FORCEINLINE FABCharacterStat GetCharacterStat(int32 InLevel) const
	{
		return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FABCharacterStat();
	}

	// 최대 레벨
	UPROPERTY()
	int32 CharacterMaxLevel;

private:
	// 스텟을 배열로 관리
	TArray<FABCharacterStat> CharacterStatTable;
};
