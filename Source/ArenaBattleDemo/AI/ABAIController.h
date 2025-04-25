// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API AABAIController : public AAIController
{
	GENERATED_BODY()
	

public:
	AABAIController();

	// AI 실행하는 함수
	void RunAI();

	// AI 중지하는 함수
	void StopAI();


protected:

	// 컨트롤러가 폰에 빙의 했을 때 실행되는 이벤트 함수 -> 이거 이후 로직을 처리하면 빙의 이후라고 보장 가능
	virtual void OnPossess(APawn* InPawn) override;

	// Data Section
private:
	// Blackboard 애셋
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	// Behavior Tree 애셋
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
