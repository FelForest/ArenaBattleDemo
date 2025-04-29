// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "AIController.h"
#include "Physics/ABCollision.h"
#include "Interface/ABCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/ABAI.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detech");

	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		return;
	}

	// 월드 정보 
	UWorld* World = ControllingPawn->GetWorld();
	//
	TArray<FOverlapResult> OverlapResults;

	// 감지 시작 위치
	FVector Center = ControllingPawn->GetActorLocation();

	// 감지 범위 설정
	float DetectRadius = AIPawn->GetAIDetectRange();

	// 충돌 제외 목록 설정
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, ControllingPawn);

	// 감지 처리
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_ABACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParams
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());

			// 플레이어인지 확인
			if (Pawn != nullptr && Pawn->GetController()->IsPlayerController())
			{
				// 감지한 플레이어를 블랙보드에 저장
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);

				// 디버그 드로우를 활용해서 감지 영역을 초록색으로 표시
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				// 감지된 지점을 녹색으로 표시
				DrawDebugPoint(World, Pawn->GetActorLocation(), 10, FColor::Green, false, 0.2f);

				// 감지된 방향으로 선 그리기
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.2f);

				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);

	// 디버그 드로우를 활용해서 감지 영역을 초록색으로 표시
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
