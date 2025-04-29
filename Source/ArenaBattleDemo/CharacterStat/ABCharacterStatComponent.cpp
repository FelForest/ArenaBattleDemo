// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"
#include "GameData/ABGameSingleton.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
	: CurrentLevel(1.0f), AttackRadius(50.0f)
{
}


// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// 레벨 스탯 데이터 설정
	SetLevelStat(static_cast<int32>(CurrentLevel));

	// 게임이 시작되면 체력을 가득 채우기
	SetHP(BaseStat.MaxHp);
}

void UABCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	// 유요한 레벨 범위로 고정
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UABGameSingleton::Get().CharacterMaxLevel);

	// 게임 싱글톤으로부터 레벨 데이터 가져오기
	BaseStat = UABGameSingleton::Get().GetCharacterStat(static_cast<int32>(CurrentLevel));

	// 설정한 값에 문제 없는지 확인
	check(BaseStat.MaxHp > 0.0f);
}

float UABCharacterStatComponent::ApplyDamage(float InDamage)
{
	// 데미지 처리
	const float PrevHP = CurrentHP;

	// 데미지 보정값
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0.0f, InDamage);

	// 데미지 계산
	SetHP(PrevHP - ActualDamage);

	// 죽었는지 (체력을 모두 소진했는지) 확인
	if (CurrentHP <= KINDA_SMALL_NUMBER)
	{
		// 죽었다는 이벤트 발행
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UABCharacterStatComponent::SetHP(float NewHP)
{
	// 현재 체력 갱신
	//CurrentHP = FMath::Clamp<float>(NewHP, 0.0f, MaxHP);
	CurrentHP = FMath::Clamp<float>(NewHP, 0.0f, BaseStat.MaxHp);

	// 체력 변경 이벤트 발행
	OnHpChanged.Broadcast(CurrentHP);
}


