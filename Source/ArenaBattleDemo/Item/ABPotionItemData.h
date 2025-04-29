// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemData.h"
#include "ABPotionItemData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API UABPotionItemData : public UABItemData
{
	GENERATED_BODY()

public:
	UABPotionItemData();

public:
	// Hp È¸º¹·®
	UPROPERTY(EditAnywhere, Category = Stat)
	float HealAmount;
	
};
