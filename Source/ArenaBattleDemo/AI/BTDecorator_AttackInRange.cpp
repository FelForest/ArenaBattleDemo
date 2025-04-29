// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"
#include "AIController.h"
#include "Interface/ABCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ABAI.h"


UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	// �����Ϳ��� ���� ��� �̸� ����
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// AIController�� �����ϴ� ��
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	// ���� ������ ����
	if (ControllingPawn == nullptr)
	{
		return false;
	}

	// �������̽��� ����ȯ
	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		return false;
	}

	// �����忡 ����� ĳ���� �� -> Ÿ��
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));

	// ��ǥ�� �Ÿ� ���
	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);

	// ���� ���� ��
	float AttackRangeWithRadius = AIPawn->GetAIAttackRange();

	// ��ǥ�� �Ÿ��� ���� ���ɰŸ����� ������� Ȯ�� �� ��� ��ȯ
	bResult = (DistanceToTarget <= AttackRangeWithRadius);

	return bResult;
}
