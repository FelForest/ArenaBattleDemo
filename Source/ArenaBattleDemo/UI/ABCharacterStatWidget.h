// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/ABCharacterStat.h"
#include "ABCharacterStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API UABCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 스탯이 업데이트될 떄 호출할 함수
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);
};
