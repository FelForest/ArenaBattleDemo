// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AttackInRange.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API UBTDecorator_AttackInRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_AttackInRange();

protected:
	// 데코레이터 실행에 필요한 함수
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
