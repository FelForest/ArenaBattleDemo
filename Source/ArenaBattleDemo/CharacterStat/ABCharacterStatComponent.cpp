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
}

float UABCharacterStatComponent::ApplyDamage(float InDamage)
{
	// 데미지 처리
	const float PrevHP = CurrentHP;

	// 데미지 보정값
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0.0f, InDamage);

	// 데미지 계산
	CurrentHP = FMath::Clamp<float>(PrevHP - ActualDamage, 0.0f, MaxHP);

	// 죽었는지 (체력을 모두 소진했는지) 확인
	if (CurrentHP <= KINDA_SMALL_NUMBER)
	{

	}

	return ActualDamage;
}


