// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"


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
