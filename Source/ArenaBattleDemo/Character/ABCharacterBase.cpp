// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "Character/ABCharacterControlData.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/ABComboActionData.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "CharacterStat/ABCharacterStatComponent.h"
#include "UI/ABWidgetComponent.h"
#include "UI/ABUserWidget.h"
#include "UI/ABHpBarWidget.h"
#include "Item/ABWeaponItemData.h"
#include "Components/SkeletalMeshComponent.h"

#include "Physics/ABCollision.h"
#include "Engine/DamageEvents.h"


// Define Log Category
DEFINE_LOG_CATEGORY(LogABCharacter);

// Sets default values
AABCharacterBase::AABCharacterBase()
	: DeadEventDelayTime(5.0f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컨트롤러의 회전을 받아서 설정하는 모드를 모두 해제
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 무드번트 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	// 컴포넌트 설정
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE);


	// 메시의 콜리전은 NoCollision 설정 (주로 랙돌에 사용됨)
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	// 리소스 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));

	// 스캘레탈 메시 설정
	if (CharacterMesh.Object != nullptr)
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}

	// 애니메이션 인스턴스 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> CharacterAnim(TEXT("/Game/ArenaBattle/Animation/ABP_ABCharacter.ABP_ABCharacter_C"));
	if (CharacterAnim.Class != nullptr)
	{
		GetMesh()->SetAnimClass((CharacterAnim.Class));
	}


	static ConstructorHelpers::FObjectFinder<UABCharacterControlData> ShoulderDataRef(TEXT("/Game/ArenaBattle/CharacterControl/ABC_Shoulder.ABC_Shoulder"));
	if (ShoulderDataRef.Object != nullptr)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UABCharacterControlData> QuarterDataRef(TEXT("/Game/ArenaBattle/CharacterControl/ABC_Quarter.ABC_Quarter"));
	if (QuarterDataRef.Object != nullptr)
	{
		CharacterControlManager.Add(ECharacterControlType::Quarter, QuarterDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object != nullptr)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	// 콤보 액션 데이터 에셋 설정
	static ConstructorHelpers::FObjectFinder<UABComboActionData> ComboActionDataRef(TEXT("/Script/ArenaBattleDemo.ABComboActionData'/Game/ArenaBattle/ComboAction/ABA_ComboAction.ABA_ComboAction'"));
	if (ComboActionDataRef.Object != nullptr)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	DeadMontage;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Game/ArenaBattle/Animation/AM_Dead.AM_Dead"));
	if (DeadMontageRef.Object != nullptr)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	// stat Component
	Stat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("Stat"));

	//
	HP = CreateDefaultSubobject<UABWidgetComponent>(TEXT("Widget"));
	HP->SetupAttachment(GetMesh());
	HP->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));

	// 사용할 위젯 클래스 정보 설정
	static ConstructorHelpers::FClassFinder<UABUserWidget> HpBarWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		// 위젯 컴포넌트는 위젯의 클래스 정보를 바탕으로 자체적으로 인스턴스를 생성함
		HP->SetWidgetClass(HpBarWidgetRef.Class);

		// 2D 모드로 그리기
		HP->SetWidgetSpace(EWidgetSpace::Screen);

		// 크기 설정
		HP->SetDrawSize(FVector2D(150.0f, 15.0f));

		// 콜리전 끄기
		HP->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Item Secion
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AABCharacterBase::EquipWeapon)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AABCharacterBase::DrinkPortion)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AABCharacterBase::ReadScroll)));

	// 무기를 보여줄 컴포넌트 생성
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));

	// 메시 컴포넌트 하위로 계층을 설정하고, 이때 hand_rSocket 소켓에 부착
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
}

void AABCharacterBase::SetCharacterControlData(const class UABCharacterControlData* InCharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = InCharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = InCharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = InCharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = InCharacterControlData->RotationRate;
}

void AABCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	ensure(ComboActionMontage != nullptr);
	ensure(ComboActionData != nullptr);
}

void AABCharacterBase::SetupCharacterWidget(UUserWidget* InUserWidget)
{
	// 필요한 위젯 정보 가져오기
	UABHpBarWidget* HpBarWidget = Cast <UABHpBarWidget>(InUserWidget);
	if (HpBarWidget != nullptr)
	{
		HpBarWidget->SetMaxHP(Stat->GetTotalStat().MaxHp);

		HpBarWidget->UpdateHpBar(Stat->GetCurrentHP());

		Stat->OnHpChanged.AddUObject(HpBarWidget, &UABHpBarWidget::UpdateHpBar);
	}
}

void AABCharacterBase::AttackHitCheck()
{
	// 공격 판정 확인

	FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();

	// 공격 거리
	//const float AttackRange = 50.0f;
	const float AttackRange = Stat->GetTotalStat().AttackRange;
	FVector End = Start + GetActorForwardVector() * AttackRange;

	// SCENE_QUERY_STAT: 언리얼에서 지원하는 분석 툴에 태그를 추가.
	// 두번째 인자: 복잡한 형태의 충돌체를 감지할 지 여부.
	// 세번째 인자: 무시할 액터 목록.
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	// 트레이스에 사용할 구체의 반지름
	const float AttackRadius = 50.0f;

	// 트레이스를 활용해 충돌 검사
	FHitResult OutHitResult;
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	// 충돌 감지된 경우 처리
	if (HitDetected)
	{
		// 데미지 양
		//const float AttackDamage = 200.0f;
		const float AttackDamage = Stat->GetTotalStat().Attack;
		// 데미지 이벤트
		FDamageEvent DamageEvent;

		// 데미지 전달
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

	// 충돌 디버그 (시각적으로 확인할 수 있도록)
#if ENABLE_DRAW_DEBUG
	
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;

	// 캡슐 높이 절반 값
	float CapsuleHalfHeight = AttackRange * 0.5f;

	// 표시할 생상 (안맞으면 빨강, 맞으면 초록)
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(
		GetWorld(), 
		CapsuleOrigin, 
		CapsuleHalfHeight, 
		AttackRadius, 
		FRotationMatrix::MakeFromX(GetActorForwardVector()).ToQuat(), 
		DrawColor, 
		false, 
		5.0f);
#endif
}

float AABCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 스텟 정보가 업데이트 되도록 데미지 전달
	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}

void AABCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 죽었을 때 발행되는 이벤트에 SetDead 함수 등록
	// BeginPlay에서 하지 않은이유
	Stat->OnHpZero.AddUObject(this, &AABCharacterBase::SetDead);
}

void AABCharacterBase::ProcessComboCommand()
{
	// 현재 재생 주인 콤보 확인
	if (CurrentCombo == 0)
	{
		// 콤보 액션 시작 처리 후 종료
		ComboActionBegin();
		return;
	}

	// 콤보가 진행 중일 때는 해당 프레임에 맞는 타이머를 활용

	// 콤보 타이머 핸들이 유요하지 않다면,
	// 이미 콤보 발동이 걸렸거나 타이임을 놓친 경우
	// 이 때는 콤보 처리가 필요하지 않음

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void AABCharacterBase::ComboActionBegin()
{
	// 콤보 상태를 1로 설정
	CurrentCombo = 1;

	// 이동 비활성화
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		//const float AttackSpeedRate = 1.0f;
		const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
		AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);
		
		// 몽타주 재생이 시작되면, 재생이 종료될 때 호출되는 델리게이트에 등록
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AABCharacterBase::ComboActionEnd);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

		// 콤보 확인을 위한 타이머 함수
		ComboTimerHandle.Invalidate();
		SetComboCheckTimer();
	}

}

void AABCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// 유효성 검사
	ensure(CurrentCombo != 0);

	// 콤보 초기화
	CurrentCombo = 0;

	// 캐릭터 무브먼트 컴포넌트 모드 복구.
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AABCharacterBase::SetComboCheckTimer()
{
	// 현재 재생 중인 콤보의 인덱스
	int32 ComboIndex = CurrentCombo - 1;

	// 콤보 인덱스 값 검증
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	// 콤보 시간 계산(확인)
	//const float AttackSpeedRate = 1.0f;
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;

	// 타이머 설정
	if (ComboEffectiveTime > 0.0f)
	{
		// 타이머 설정
		GetWorldTimerManager().SetTimer(ComboTimerHandle, this, &AABCharacterBase::ComboCheck, ComboEffectiveTime, false);
		// GetWorld()->GetTimerManager(); // GetWorldTimerManager() 내부가 이렇게 되어 있음
	}
}

void AABCharacterBase::ComboCheck()
{
	// 타이머 핸들 무효화(초기화)
	ComboTimerHandle.Invalidate();

	// 이전에 공격 입력이 들어왔는지 확인
	if (HasNextComboCommand)
	{
		// 몽타주 점프
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// 다음 콤보 인덱스 설정
			CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);

			// 점프할 섹션의 이름 설정(예: ComboAttack2).
			FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);

			// 섹션 점프
			AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);

			// 다음 콤보 공격을 위한 타이머 설정
			SetComboCheckTimer();

			// 콤보 공격 입력 플래그 초기화
			HasNextComboCommand = false;
		}
	}
}

void AABCharacterBase::SetDead()
{
	// 무브먼트 컴포넌트 끄기
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// 콜리전 끄기
	SetActorEnableCollision(false);

	// 죽는 애니메이션 재생
	PlayDeadAnimation();

	// 죽었을 때 HPBar(위젯) 사라지도록 처리
	HP->SetHiddenInGame(true);
}

void AABCharacterBase::PlayDeadAnimation()
{
	// 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		// 이미 재생 중인 몽타주가 있다면, 모두 종료
		AnimInstance->StopAllMontages(0.0f);

		// 죽음 몽타주 재생
		const float PlayRate = 1.0f;
		AnimInstance->Montage_Play(DeadMontage, PlayRate);
	}
}

void AABCharacterBase::TakeItem(UABItemData* InItemData)
{
	/*switch (InItemData->Type)
	{
	case EItemType::Weapon:
	{

	}
	break;
	}*/

	if (InItemData)
	{
		TakeItemActions[static_cast<uint8>(InItemData->Type)].ItemDelegate.ExecuteIfBound(InItemData);
	}
}

void AABCharacterBase::DrinkPortion(UABItemData* InItemData)
{
	UE_LOG(LogABCharacter, Log, TEXT("Drink Portion"));
}

void AABCharacterBase::EquipWeapon(UABItemData* InItemData)
{
	//UE_LOG(LogABCharacter, Log, TEXT("Equip Weapon"));
	// 함수에 전달된 아이템 데이터 에셋을 무기 데이터로 변환
	UABWeaponItemData* WeaponItemData = Cast<UABWeaponItemData>(InItemData);
	if (WeaponItemData != nullptr)
	{
		// 무기 메시가 아직 로딩 안도니 경우 로드 처리
		if (WeaponItemData->WeaponMesh.IsPending())
		{
			// 아직 로드가 안된거임
			// 이거 동기로 하는 거임
			WeaponItemData->WeaponMesh.LoadSynchronous();
		}
		// 무기 컴포넌트에 로드가 완료된 스켈레탈 메시 설정
		Weapon->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());

		// 무기 아이템 데이터가 가지는 부가 스탯 설정
		Stat->SetModifierStat(WeaponItemData->ModifierStat);
	}
}

void AABCharacterBase::ReadScroll(UABItemData* InItemData)
{
	UE_LOG(LogABCharacter, Log, TEXT("Read Scroll"));
}

int32 AABCharacterBase::GetLevel() const
{
	return Stat->GetCurrentLevel();
}

void AABCharacterBase::SetLevel(int32 InNewLevel)
{
	Stat->SetLevelStat(InNewLevel);
}


