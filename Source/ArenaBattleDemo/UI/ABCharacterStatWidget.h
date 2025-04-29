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
	
protected:
	virtual void NativeConstruct() override;

public:
	// 스탯이 업데이트될 떄 호출할 함수
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);

private:
	// 스탯 위젯이 가진 UI 컨트롤 중에 Base 스탯에 해당하는 텍스트 블록을 저장하는 맵
	UPROPERTY()
	TMap<FName, class UTextBlock*> BaseLookup;

	// 스탯 위젯이 가진 UI 컨트롤 중에 Modifier 스탯에 해당하는 텍스트 블록을 저장하는 맵
	UPROPERTY()
	TMap<FName, class UTextBlock*> ModifierLookup;
};
