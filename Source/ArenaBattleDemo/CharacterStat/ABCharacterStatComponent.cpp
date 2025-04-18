// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
	: MaxHP(200.0f), CurrentHP(MaxHP)
{

}


// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// 게임이 시작되면 체력을 가득 채우기
	SetHP(MaxHP);
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
	CurrentHP = FMath::Clamp<float>(NewHP, 0.0f, MaxHP);

	// 체력 변경 이벤트 발행
	OnHpChanged.Broadcast(CurrentHP);
}


