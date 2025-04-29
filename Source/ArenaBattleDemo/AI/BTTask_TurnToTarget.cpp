// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ABAI.h"
#include "Interface/ABCharacterAIInterface.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	// 노드 이름 설정
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 회전을 하기 위해서는 목표 위치와 현재 AI의 위치를 알아야 함
	// AI의 회전을 하기 위해서는 AI to Target Vector 랑 Foward Vector를 내적해서 구하면 되는데 역함수를 구하라고하는데 좀더 알아보도록
	// 얼마나 돌지는 알았지만 어디로 돌지는 외적으로 구하면 된다

	// AIController가 제어하는 폰
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 플레이어
	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));

	if (TargetPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 인터페이스 형변환
	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);

	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 회전할 속도 값
	float TrunSpeed = AIPawn->GetAITurnSpeed();

	// 바라볼 방향 구하기
	FVector LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();

	// 높이 성분은 제거
	LookVector.Z = 0.0f;

	// 바라볼 방향을 이용해 FRotator 구하기
	FRotator TargetRotator = FRotationMatrix::MakeFromX(LookVector).Rotator();

	// 부드럽게 회전 설정

	// 현재 프레임에 설정할 회전 값
	FRotator CurrentRotator = FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRotator, GetWorld()->DeltaTimeSeconds, TrunSpeed);

	// 회전값 설정
	ControllingPawn->SetActorRotation(CurrentRotator);

	return EBTNodeResult::Type();
}
