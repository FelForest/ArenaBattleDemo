// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterNonPlayer.h"
#include "Engine/AssetManager.h"
#include "AI/ABAIController.h"
#include "Interface/ABCharacterAIInterface.h"
#include "CharacterStat/ABCharacterStatComponent.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
	// 시작할 때 메시 컴포넌트 끄기
	GetMesh()->SetHiddenInGame(true);

	// AIControllerClass 설정
	AIControllerClass = AABAIController::StaticClass();

	// 빙의 모드 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AABCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 메시로드 요청 작업

	// NPCMeshes 목록 값이 제대로 설정 됐는지 확인
	ensure(NPCMeshes.Num() > 0);

	// 랜덤으로 인덱스 선택
	int32 RandomIndex = FMath::RandRange(0, NPCMeshes.Num() - 1);

	// 비동기로 메시 로드 요청
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandomIndex], FStreamableDelegate::CreateUObject(this, &AABCharacterNonPlayer::NPCMeshLoadCompleted));
}

void AABCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	// 타이머를 사용해 액터 제거
	FTimerHandle DeadTimerHandle;
	GetWorldTimerManager().SetTimer(
		DeadTimerHandle,
		FTimerDelegate::CreateLambda([&]()
			{
				// 액터 제거
				Destroy();
			}),
		DeadEventDelayTime,
		false
	);
}

void AABCharacterNonPlayer::NPCMeshLoadCompleted()
{
	// 메시 요청 시 반환된 핸들 값이 유효한지 확인
	if (NPCMeshHandle.IsValid())
	{
		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset());

		if (NPCMesh != nullptr)
		{
			// 스켈레탈 메시 애셋 설정
			GetMesh()->SetSkeletalMesh(NPCMesh);

			// 컴포넌트 보이도록 설정
			GetMesh()->SetHiddenInGame(false);
		}
	}

	// 에셋 로드에 사용 했던 핸들 반환
	NPCMeshHandle->ReleaseHandle();
}

float AABCharacterNonPlayer::GetAIPatrolRadious()
{
	return 800.0f;
}

float AABCharacterNonPlayer::GetAIDetectRange()
{
	return 400.0f;
}

float AABCharacterNonPlayer::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius() * 2;
}

float AABCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

void AABCharacterNonPlayer::SetAIAttackDelegeate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AABCharacterNonPlayer::AttackbyAI()
{
	// 공격 진행을 위한 콤보 실행 함수 호출
	ProcessComboCommand();
}

void AABCharacterNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();

	// 전달  받은 델리게이트 실행
	OnAttackFinished.ExecuteIfBound();
}
