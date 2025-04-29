// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ABAIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/ABAI.h"

AABAIController::AABAIController()
{
	// 애셋 로드
	// 블랙 보드 애셋
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/ArenaBattle/AI/BB_ABCharacter.BB_ABCharacter'"));
	if (BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	// 행동 트리 애셋
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/ArenaBattle/AI/BT_ABCharacter.BT_ABCharacter'"));
	if (BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AABAIController::RunAI()
{
	// 블랙보드 컴포넌트 포인터 가져오기
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();

	// 사용할 블랙보드 지정
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		// 시작할 때 NPC의 위치를 블랙보드에 HomePosition에 저장
		Blackboard->SetValueAsVector(BBKEY_HOMEPOSITION, GetPawn()->GetActorLocation());

		// 행동 트리 실행
		bool RunResult = RunBehaviorTree(BTAsset);

		// 실행에 실패하면 확인
		ensure(RunResult);
	}
}

void AABAIController::StopAI()
{
	// 중지를 위해 실행 중인 behaviorTreeComponenet 가져오기
	UBehaviorTreeComponent* BTComponenet = Cast<UBehaviorTreeComponent>(BrainComponent);

	// BT 중지
	if (BTComponenet != nullptr)
	{
		BTComponenet->StopTree();
	}
}

void AABAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 컨트롤러가 폰에 빙의하면 AI를 실행
	RunAI();
}
