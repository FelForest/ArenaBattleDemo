// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/ABCharacterStat.h"
#include "ABCharacterStatComponent.generated.h"


// 델리게이트 선언
// 체력 값이 0이 되었을 때 발행할 델리게이트.
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);

// 체력 변경이 발생할 때 발행할 델리게이트.
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHP*/);

// 스탯 정보 변경이 발생할 때 발행할 델리게이트
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FABCharacterStat&, const FABCharacterStat&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLEDEMO_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Getter
	//FORCEINLINE float GetMaxHP() { return MaxHP; }
	FORCEINLINE float GetCurrentHP() { return CurrentHP; }

	FORCEINLINE float GetAttackRadius() const { return AttackRadius; };

	// 캐릭터 레벨을 설정하는 함수
	void SetLevelStat(int32 InNewLevel);
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }

	// 전체 스탯 데이터 반환 함수
	FORCEINLINE FABCharacterStat GetTotalStat() const
	{
		return BaseStat + ModifierStat;
	}

	// 기본 스탯 정보가 변경될 때 사용할 함수
	FORCEINLINE void SetBaseStat(const FABCharacterStat& InBaseStat)
	{
		BaseStat = InBaseStat;
		OnStatChanged.Broadcast(BaseStat, ModifierStat);
	}

	// 부가 스탯 정보가 변경될 때 사용할 함수
	FORCEINLINE void SetModifierStat(const FABCharacterStat& InModifierStat)
	{
		ModifierStat = InModifierStat;
		OnStatChanged.Broadcast(BaseStat, ModifierStat);
	}

	FORCEINLINE const FABCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FABCharacterStat& GetModifierStat() const { return ModifierStat; }

	float ApplyDamage(float InDamage);

protected:
	// HP가 변경 됐을 때 실행할 함수
	void SetHP(float NewHP);

public:
	// 체력을 모두 소진했을 때 발행되는 델리게이트
	FOnHpZeroDelegate OnHpZero;

	// 체력 변경 델리게이트
	FOnHpChangedDelegate OnHpChanged;

	// 스탯 변경 델리게이트
	FOnStatChangedDelegate OnStatChanged;
protected:

	// 임시값으로 데이터 제거
	/*UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHP;*/

	// Transient : 현재 체ㅔ력 값은 게임을 진행할 따마다 바뀌는 값
	// 따라서 디스크에 명시적으로 저장할 필요하지 않을 수 있음
	// 이럴 때는 transient로 지정 가능
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHP;
	

	// 현재 레벨
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	// 공격 범위
	UPROPERTY(VisibleInstanceOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	// 캐릭터의 기본 스탯 데이터
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	FABCharacterStat BaseStat;

	// 부가 스텟 데이터
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	FABCharacterStat ModifierStat;
};
