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

	// 리소스 설정
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

		// 생성한 스태틱 메시 컴포넌트에 애샛 설정
		if (GateMeshRef.Succeeded())
		{
			Gate->SetStaticMesh(GateMeshRef.Object);
		}

		// Setting Component
		Gate->SetupAttachment(Stage, GateSocket);
		Gate->SetRelativeLocation(FVector(0.0f, -80.0f, 0.0f));
		Gate->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

		// 생성한 컴포넌트를 맵에 추가
		Gates.Add(GateSocket, Gate);

		// 문마다 배치할 BoxComponent 생성 및 설정
		FName TriggerName = *GateSocket.ToString().Append("Trigger");

		// 컴포넌트 생성
		UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);

		// 컴포넌트 설정
		GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		GateTrigger->SetupAttachment(Stage, GateSocket);
		GateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
		GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnGateTriggerBeginOverlap);

		// 태그 설정
		GateTrigger->ComponentTags.Add(GateSocket);

		// 생성한 컴포넌트를 배열에 추가
		GateTriggers.Add(GateTrigger);

	}

	// 시작할 때는 준비 상태로 설정
	CurrentState = EStageState::Ready;

	//  열거형 - 델리게이트 맵 설정
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

	// OnConstruction이 호출될 때 상태 값도 갱싱되도록 처리
	SetState(CurrentState);
}

void AABStageGimmick::SetState(EStageState InNewState)
{
	// 현재 상태 업데이트
	CurrentState = InNewState;

	// 전달된 상태에 맵에 포함돼 있으면 델리게이트 실행
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
	// 캐릭터가 스테이지에 입장하면 대전 상태로 전환
	SetState(EStageState::Fight);
}

void AABStageGimmick::SetReady()
{
	// 가운데 트리거 활성화
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);

	// 플레이어가 게이트와 상화작용하지 않도록 콜리전 끄기
	for (const auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// 준비 상태일 때는 문이 열려있도록 설정
	OpenAllGates();
}

void AABStageGimmick::SetFight()
{
	// 가운데 트리거 활성화
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	// 플레이어가 게이트와 상화작용하지 않도록 콜리전 끄기
	for (const auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// 모든 문 닫기
	CloseAllGates();

	// NPC 생성
	GetWorldTimerManager().SetTimer(OpponentTimerHandle, this, &AABStageGimmick::OpponentSpawn, OpponentSpawnTime, false);
}

void AABStageGimmick::SetChooseReward()
{
	// 가운데 트리거 활성화
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	// 플레이어가 게이트와 상화작용하지 않도록 콜리전 끄기
	for (const auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// 아이템 생성 로직
	
	// 모든 문 닫기
	CloseAllGates();
}

void AABStageGimmick::SetChooseNext()
{
	// 가운데 트리거 활성화
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	// 플레이어가 게이트와 상화작용하지 않도록 콜리전 끄기
	for (const auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	}

	// 모든 문 닫기
	OpenAllGates();
}

void AABStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 게이트에는 하나의 태그를 설정했기 때문에 이를 확인.
	ensure(OverlappedComponent->ComponentTags.Num() == 1);

	// 태그 확인 (예: +XGate)
	FName ComponentTag = OverlappedComponent->ComponentTags[0];

	// 태그에서 스테이지를 배치할 소켓의 이름을 가져오기.
	FName SocketName = FName(*ComponentTag.ToString().Left(2));

	// 소켓이 있는지 확인.
	check(Stage->DoesSocketExist(SocketName));

	// 소켓 이름을 통해 위치 값 가져오기.
	FVector NewLocation = Stage->GetSocketLocation(SocketName);

	// 가져온 위치에 이미 다른 스테이지가 없는지 확인.
	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams CollisionQueryParams(
		SCENE_QUERY_STAT(GateTrigger),
		false,
		this
	);

	// 오버랩으로 검사.
	bool Result = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,        // 충돌 결과를 반환할 변수.
		NewLocation,        // 충돌 판정할 위치.
		FQuat::Identity,    // 회전.
		// 충돌 대상 오브젝트 채널.
		FCollisionObjectQueryParams::InitType::AllStaticObjects,
		// 충돌 판정할 때 사용할 모형.
		FCollisionShape::MakeSphere(775.0f),
		// 콜리전 옵션(자기는 제외하기 위해).
		CollisionQueryParams
	);

	if (!Result)
	{
		GetWorld()->SpawnActor<AABStageGimmick>(NewLocation, FRotator::ZeroRotator);
	}
}

void AABStageGimmick::OpenAllGates()
{
	// 문 액터의 배열을 순회하면서 회전 설정
	for (const auto& Gate : Gates)
	{
		Gate.Value->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
}

void AABStageGimmick::CloseAllGates()
{
	// 문 액터의 배열을 순회하면서 회전 설정
	for (const auto& Gate : Gates)
	{
		Gate.Value->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

void AABStageGimmick::OpponentDestroyed(AActor* DestroyedActor)
{
	// NPC가 죽으면 보상 단계로 설정
	SetState(EStageState::Reward);
}

void AABStageGimmick::OpponentSpawn()
{
	// NPC를 생성할 위치 설정
	const FVector SpawnLocation = GetActorLocation() + FVector::UpVector * 88.0f;
	// NPC 생성
	AActor* OpponentActor = GetWorld()->SpawnActor(OpponentClass, &SpawnLocation, &FRotator::ZeroRotator);

	// NPC가 죽었을때 발행되는 델리게이트에 등록
	AABCharacterNonPlayer* ABOpponentCharacter = Cast<AABCharacterNonPlayer>(OpponentActor);

	if (ABOpponentCharacter != nullptr)
	{
		ABOpponentCharacter->OnDestroyed.AddDynamic(this, &AABStageGimmick::OpponentDestroyed);
	}
}
