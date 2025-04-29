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

	// ���� ���� 
	UWorld* World = ControllingPawn->GetWorld();
	//
	TArray<FOverlapResult> OverlapResults;

	// ���� ���� ��ġ
	FVector Center = ControllingPawn->GetActorLocation();

	// ���� ���� ����
	float DetectRadius = AIPawn->GetAIDetectRange();

	// �浹 ���� ��� ����
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, ControllingPawn);

	// ���� ó��
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

			// �÷��̾����� Ȯ��
			if (Pawn != nullptr && Pawn->GetController()->IsPlayerController())
			{
				// ������ �÷��̾ �����忡 ����
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);

				// ����� ��ο츦 Ȱ���ؼ� ���� ������ �ʷϻ����� ǥ��
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				// ������ ������ ������� ǥ��
				DrawDebugPoint(World, Pawn->GetActorLocation(), 10, FColor::Green, false, 0.2f);

				// ������ �������� �� �׸���
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.2f);

				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);

	// ����� ��ο츦 Ȱ���ؼ� ���� ������ �ʷϻ����� ǥ��
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
