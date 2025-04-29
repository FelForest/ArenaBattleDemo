// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ABAI.h"
#include "Interface/ABCharacterAIInterface.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	// ��� �̸� ����
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// ȸ���� �ϱ� ���ؼ��� ��ǥ ��ġ�� ���� AI�� ��ġ�� �˾ƾ� ��
	// AI�� ȸ���� �ϱ� ���ؼ��� AI to Target Vector �� Foward Vector�� �����ؼ� ���ϸ� �Ǵµ� ���Լ��� ���϶���ϴµ� ���� �˾ƺ�����
	// �󸶳� ������ �˾����� ���� ������ �������� ���ϸ� �ȴ�

	// AIController�� �����ϴ� ��
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// �÷��̾�
	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));

	if (TargetPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// �������̽� ����ȯ
	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);

	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// ȸ���� �ӵ� ��
	float TrunSpeed = AIPawn->GetAITurnSpeed();

	// �ٶ� ���� ���ϱ�
	FVector LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();

	// ���� ������ ����
	LookVector.Z = 0.0f;

	// �ٶ� ������ �̿��� FRotator ���ϱ�
	FRotator TargetRotator = FRotationMatrix::MakeFromX(LookVector).Rotator();

	// �ε巴�� ȸ�� ����

	// ���� �����ӿ� ������ ȸ�� ��
	FRotator CurrentRotator = FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRotator, GetWorld()->DeltaTimeSeconds, TrunSpeed);

	// ȸ���� ����
	ControllingPawn->SetActorRotation(CurrentRotator);

	return EBTNodeResult::Type();
}
