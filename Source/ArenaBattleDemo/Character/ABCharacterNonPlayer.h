// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/ABCharacterAIInterface.h"
#include "ABCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS(config=ArenaBattle)
class ARENABATTLEDEMO_API AABCharacterNonPlayer : 
	public AABCharacterBase,
	public IABCharacterAIInterface
{
	GENERATED_BODY()

public:
	AABCharacterNonPlayer();

protected:

	virtual void PostInitializeComponents() override;

	// 죽음 샅애 설정 함수
	virtual void SetDead() override;

	// NPC 메시 로드가 완료될 때 호출될 콜백
	void NPCMeshLoadCompleted();
	
	// config 파일에 설정된 경로 값을 배열로 관리
	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;

	// 배열에 채워진 경로를 활용해 비동기로 애셋을 로드할 때 사용
	TSharedPtr<FStreamableHandle> NPCMeshHandle;

protected:
	// IABCharacterAIInterface 인터페이스 함수 구현
	virtual float GetAIPatrolRadious() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	// 캐릭터에게 델리게이트를 넘길 때 사용할 함수
	virtual void SetAIAttackDelegeate(const FAICharacterAttackFinished& InOnAttackFinished) override;

	// AI가 공격을 할 때 사용할 함수
	virtual void AttackbyAI() override;

	//
	FAICharacterAttackFinished OnAttackFinished;

	virtual void NotifyComboActionEnd() override;

};
