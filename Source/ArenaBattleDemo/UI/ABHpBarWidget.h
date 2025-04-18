// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABUserWidget.h"
#include "ABHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API UABHpBarWidget : public UABUserWidget
{
	GENERATED_BODY()
	
public:
	UABHpBarWidget(const FObjectInitializer& ObjectInitializer);

	// 최대 체력 설정 함수
	FORCEINLINE void SetMaxHP(float NewMaxHP) { MaxHP = NewMaxHP; }

	// HpBar에 퍼센트를 조정할 때 호출할 함수
	void UpdateHpBar(float NewCurrentHP);


protected:
	virtual void NativeConstruct() override;

protected:
	// HP 게이지를 보여주기 위해 사용할 프로그레스바 참조 변수
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	// 체대 체력 값 // stat component에서 바로 가져오는게 아니라 캐릭터에서 가져오는 방식으로 할 것임
	UPROPERTY()
	float MaxHP;
};
