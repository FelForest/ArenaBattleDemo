// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AICOntroller.h"
#include "Interface/ABCharacterAIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// AIController가 제저하는 폰
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 인터페이스로 형변환
	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);

	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 캐릭터가 공격ㄱ을 종료할 때 실행할 델리게이트 선언
	FAICharacterAttackFinished OnAttackFInished;

	// 람다 함수를 상숑해 델리게이트에 바인딩
	OnAttackFInished.BindLambda(
		[&]()
		{
			// 캐릭터의 공격이 끝날 때 테스크를 끝나는 값을 전달
			// InProgress로 반환한 후에 태스크를 최종 마무리할 때는
			// FinishLatenTask 함수를 사용 가능 -> InProgress로 반환이 되어 있어야 가능
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	// 캐릭터가 공격을 종료할 때 실행할 델리게이트 전달(설정)
	AIPawn->SetAIAttackDelegeate(OnAttackFInished);

	// 공격 명령 실행
	AIPawn->AttackbyAI();

	// AI가 공격을 하는데 애니메이션 같은 시간이 걸리는것 때문에 곧바로 결과를 알 수 없기 때문에
	// 진행중(InProgress) 값 반환
	return EBTNodeResult::InProgress;
}
