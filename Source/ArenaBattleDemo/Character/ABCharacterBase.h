// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ABAnimationAttackInterface.h"
#include "Interface/ABCharacterWidgetInterface.h"
#include "Interface/ABCHaracterItemInterface.h"
#include "ABCharacterBase.generated.h"

// 로그 카테고리 추가
DECLARE_LOG_CATEGORY_EXTERN(LogABCharacter, Log, All);

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quarter,
};

// 아이템 획득 처리를 위한 델리게이트 선언
DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UABItemData* /*InItemData*/);

// 델리게이트를 다수의 배열(맵)로 관리하기 위한 구조체 선언
// 델리게이트 자체를 인자로 사용할 수 없기 때문에 래퍼 구조체 선언이 필요함
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()

	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate)
		: ItemDelegate(InItemDelegate) {}

	FOnTakeItemDelegate ItemDelegate;
};

UCLASS()
class ARENABATTLEDEMO_API AABCharacterBase : 
	public ACharacter, 
	public IABAnimationAttackInterface, 
	public IABCharacterWidgetInterface, 
	public IABCharacterItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacterBase();

	virtual void SetCharacterControlData(const class UABCharacterControlData* InCharacterControlData);

	virtual void BeginPlay() override;

	virtual void SetupCharacterWidget(class UUserWidget* InUserWidget) override;
	// 공격 감지 함수 (애님 노티파이로부터 호출됨)
	virtual void AttackHitCheck() override;

	// 데미지 처리함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	// 컴포넌트가 초기화된 이후에 호출되는 함수
	virtual void PostInitializeComponents() override;

protected:
	// 콤보 액션 처리 함수
	// 공격을 처음 재생할 때와 콤보 액션 처리를 분기
	void ProcessComboCommand();

	// 콤보 액션이 시작될 때 호출할 함수
	void ComboActionBegin();

	// 콤보가 종료될 때 호출될 함수
	// 애님 몽타주에서 제공하는 델리게이트와 파라미터 맞춤
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	// 콤보 타이머 설정 함수
	void SetComboCheckTimer();

	// 타이머 시간 사이에 입력이 들어왔는지 여부를 확인하는 함수
	void ComboCheck();

	// Dead Section
protected:
	virtual void SetDead();

	// 죽는 애니메이션 재생 함수
	void PlayDeadAnimation();
protected:
	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;

	// 공격 몽타주 애셋.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	// 콤보 처리시 사용할 데이터 애셋.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABComboActionData> ComboActionData;

	// 현재 재생 중인 콤보 단계.
	// 0-> 콤보 시작하지 않음. 1/2/3/4 콤보가 시작됨.
	int32 CurrentCombo = 0;

	// 콤보 가능 여부를 판단하기 위한 타이머 핸들.
	FTimerHandle ComboTimerHandle;

	// 콤보 타이머 이전에 입력이 들어왔는지를 확인하는 불리언 변수.
	bool HasNextComboCommand = false;


	// Dead Section
protected:
	// 죽음 몽타주 에셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	// 죽은 뒤에 액터를 제거하기 전까지 대기할 시간 값
	float DeadEventDelayTime;

	// Stat / Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABCharacterStatComponent> Stat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABWidgetComponent> HP;

	// Item Section
protected:
	// 래퍼 구조체를 관리할 수 있는 배열
	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	// 아이템 획득 시 호출될 함수
	virtual void TakeItem(UABItemData* InItemData) override;

	// 아이템 종류마다 처리될 함수 선언
	virtual void DrinkPortion(class UABItemData* InItemData);
	virtual void EquipWeapon(class UABItemData* InItemData);
	virtual void ReadScroll(class UABItemData* InItemData);

	// 무기 아이템을 획득했을 때 사용할 스켈레탈 메시 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	// Stat Section
public:

	// 레벨 Getter/Setter
	int32 GetLevel() const;

	void SetLevel(int32 InNewLevel);
};
