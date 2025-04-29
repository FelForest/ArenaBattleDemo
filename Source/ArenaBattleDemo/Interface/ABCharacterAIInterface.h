// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ABCharacterAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UABCharacterAIInterface : public UInterface
{
	GENERATED_BODY()
};

// 공격 종료 델리게이트.
DECLARE_DELEGATE(FAICharacterAttackFinished);

/**
 * 
 */
class ARENABATTLEDEMO_API IABCharacterAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// NPC 캐릭터가 필수적으로 구현해야할 함수
	virtual float GetAIPatrolRadious() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;

	// 캐릭터에게 델리게이트를 넘길 때 사용할 함수
	virtual void SetAIAttackDelegeate(const FAICharacterAttackFinished& InOnAttackFinished) = 0;
	
	// AI가 공격을 할 때 사용할 함수
	virtual void AttackbyAI() = 0;


};
