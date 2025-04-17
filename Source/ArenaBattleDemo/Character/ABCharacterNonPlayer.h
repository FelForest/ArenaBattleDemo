// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "ABCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API AABCharacterNonPlayer : public AABCharacterBase
{
	GENERATED_BODY()

public:
	AABCharacterNonPlayer();

protected:
	// Á×À½ »ô¾Ö ¼³Á¤ ÇÔ¼ö
	virtual void SetDead() override;
	
};
