// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"


// 델리게이트 선언
// 체력 값이 0이 되었을 때 발행할 델리게이트.
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);

// 체력 변경이 발생할 때 발행할 델리게이트.
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHP*/);

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
	FORCEINLINE float GetMaxHP() { return MaxHP; }
	FORCEINLINE float GetCurrentHP() { return CurrentHP; }

	float ApplyDamage(float InDamage);

protected:
	// HP가 변경 됐을 때 실행할 함수
	void SetHP(float NewHP);

public:
	// 체력을 모두 소진했을 때 발행되는 델리게이트
	FOnHpZeroDelegate OnHpZero;

	// 체력 변경 델리게이트
	FOnHpChangedDelegate OnHpChanged;

	// 스탯
protected:
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHP;

	// Transient : 현재 체ㅔ력 값은 게임을 진행할 따마다 바뀌는 값
	// 따라서 디스크에 명시적으로 저장할 필요하지 않을 수 있음
	// 이럴 때는 transient로 지정 가능
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHP;
		
};
