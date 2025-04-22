// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ABStageGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/ABCollision.h"
#include "Character/ABCharacterNonPlayer.h"

// Sets default values
AABStageGimmick::AABStageGimmick()
{
	// State Section
	Stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	RootComponent = Stage;

	// ���ҽ� ����
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshRef(TEXT("/Game/ArenaBattle/Environment/Stages/SM_SQUARE.SM_SQUARE"));
	if (StageMeshRef.Object)
	{
		Stage->SetStaticMesh(StageMeshRef.Object);
	}

	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	StageTrigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	StageTrigger->SetupAttachment(Stage);
	StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnStageTriggerBeginOverlap);

	// Gate Section
	static FName GateSockets[] = { TEXT("+XGate"), TEXT("-XGate") , TEXT("+YGate") ,TEXT("-YGate") };
	
	// resource setting
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Game/ArenaBattle/Environment/Props/SM_GATE.SM_GATE"));

	for (const FName& GateSocket : GateSockets)
	{
		// Create Component
		UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);

		// ������ ����ƽ �޽� ������Ʈ�� �ֻ� ����
		if (GateMeshRef.Succeeded())
		{
			Gate->SetStaticMesh(GateMeshRef.Object);
		}

		// Setting Component
		Gate->SetupAttachment(Stage, GateSocket);
		Gate->SetRelativeLocation(FVector(0.0f, -80.0f, 0.0f));
		Gate->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

		// ������ ������Ʈ�� �ʿ� �߰�
		Gates.Add(GateSocket, Gate);

		// ������ ��ġ�� BoxComponent ���� �� ����
		FName TriggerName = *GateSocket.ToString().Append("Trigger");

		// ������Ʈ ����
		UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);

		// ������Ʈ ����
		GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		GateTrigger->SetupAttachment(Stage, GateSocket);
		GateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
		GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnGateTriggerBeginOverlap);

		// �±� ����
		GateTrigger->ComponentTags.Add(GateSocket);

		// ������ ������Ʈ�� �迭�� �߰�
		GateTriggers.Add(GateTrigger);

	}

	// ������ ���� �غ� ���·� ����
	CurrentState = EStageState::Ready;

	//  ������ - ��������Ʈ �� ����
	StageChangedActions.Add(EStageState::Ready, FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetReady));
	StageChangedActions.Add(EStageState::Fight, FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetFight));
	StageChangedActions.Add(EStageState::Reward, FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetChooseReward));
	StageChangedActions.Add(EStageState::Next, FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetChooseNext));

	// Fight Section
	OpponentSpawnTime = 2.0f;
	OpponentClass = AABCharacterNonPlayer::StaticClass();

}

void AABStageGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// OnConstruction�� ȣ��� �� ���� ���� ���̵ǵ��� ó��
	SetState(CurrentState);
}

void AABStageGimmick::SetState(EStageState InNewState)
{
	// ���� ���� ������Ʈ
	CurrentState = InNewState;

	// ���޵� ���¿� �ʿ� ���Ե� ������ ��������Ʈ ����
	if (StageChangedActions.Contains(InNewState))
	{
		StageChangedActions[CurrentState].StageChangedDelegate.ExecuteIfBound();
	}
}

void AABStageGimmick::OnStageTriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	// ĳ���Ͱ� ���������� �����ϸ� ���� ���·� ��ȯ
	SetState(EStageState::Fight);
}

void AABStageGimmick::SetReady()
{
	// ��� Ʈ���� Ȱ��ȭ
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);

	// �÷��̾ ����Ʈ�� ��ȭ�ۿ����� �ʵ��� �ݸ��� ����
	for (const auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// �غ� ������ ���� ���� �����ֵ��� ����
	OpenAllGates();
}

void AABStageGimmick::SetFight()
{
	// ��� Ʈ���� Ȱ��ȭ
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	// �÷��̾ ����Ʈ�� ��ȭ�ۿ����� �ʵ��� �ݸ��� ����
	for (const auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// ��� �� �ݱ�
	CloseAllGates();

	// NPC ����
	GetWorldTimerManager().SetTimer(OpponentTimerHandle, this, &AABStageGimmick::OpponentSpawn, OpponentSpawnTime, false);
}

void AABStageGimmick::SetChooseReward()
{
	// ��� Ʈ���� Ȱ��ȭ
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	// �÷��̾ ����Ʈ�� ��ȭ�ۿ����� �ʵ��� �ݸ��� ����
	for (const auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// ������ ���� ����
	
	// ��� �� �ݱ�
	CloseAllGates();
}

void AABStageGimmick::SetChooseNext()
{
	// ��� Ʈ���� Ȱ��ȭ
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	// �÷��̾ ����Ʈ�� ��ȭ�ۿ����� �ʵ��� �ݸ��� ����
	for (const auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	}

	// ��� �� �ݱ�
	OpenAllGates();
}

void AABStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ����Ʈ���� �ϳ��� �±׸� �����߱� ������ �̸� Ȯ��.
	ensure(OverlappedComponent->ComponentTags.Num() == 1);

	// �±� Ȯ�� (��: +XGate)
	FName ComponentTag = OverlappedComponent->ComponentTags[0];

	// �±׿��� ���������� ��ġ�� ������ �̸��� ��������.
	FName SocketName = FName(*ComponentTag.ToString().Left(2));

	// ������ �ִ��� Ȯ��.
	check(Stage->DoesSocketExist(SocketName));

	// ���� �̸��� ���� ��ġ �� ��������.
	FVector NewLocation = Stage->GetSocketLocation(SocketName);

	// ������ ��ġ�� �̹� �ٸ� ���������� ������ Ȯ��.
	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams CollisionQueryParams(
		SCENE_QUERY_STAT(GateTrigger),
		false,
		this
	);

	// ���������� �˻�.
	bool Result = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,        // �浹 ����� ��ȯ�� ����.
		NewLocation,        // �浹 ������ ��ġ.
		FQuat::Identity,    // ȸ��.
		// �浹 ��� ������Ʈ ä��.
		FCollisionObjectQueryParams::InitType::AllStaticObjects,
		// �浹 ������ �� ����� ����.
		FCollisionShape::MakeSphere(775.0f),
		// �ݸ��� �ɼ�(�ڱ�� �����ϱ� ����).
		CollisionQueryParams
	);

	if (!Result)
	{
		GetWorld()->SpawnActor<AABStageGimmick>(NewLocation, FRotator::ZeroRotator);
	}
}

void AABStageGimmick::OpenAllGates()
{
	// �� ������ �迭�� ��ȸ�ϸ鼭 ȸ�� ����
	for (const auto& Gate : Gates)
	{
		Gate.Value->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
}

void AABStageGimmick::CloseAllGates()
{
	// �� ������ �迭�� ��ȸ�ϸ鼭 ȸ�� ����
	for (const auto& Gate : Gates)
	{
		Gate.Value->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

void AABStageGimmick::OpponentDestroyed(AActor* DestroyedActor)
{
	// NPC�� ������ ���� �ܰ�� ����
	SetState(EStageState::Reward);
}

void AABStageGimmick::OpponentSpawn()
{
	// NPC�� ������ ��ġ ����
	const FVector SpawnLocation = GetActorLocation() + FVector::UpVector * 88.0f;
	// NPC ����
	AActor* OpponentActor = GetWorld()->SpawnActor(OpponentClass, &SpawnLocation, &FRotator::ZeroRotator);

	// NPC�� �׾����� ����Ǵ� ��������Ʈ�� ���
	AABCharacterNonPlayer* ABOpponentCharacter = Cast<AABCharacterNonPlayer>(OpponentActor);

	if (ABOpponentCharacter != nullptr)
	{
		ABOpponentCharacter->OnDestroyed.AddDynamic(this, &AABStageGimmick::OpponentDestroyed);
	}
}
