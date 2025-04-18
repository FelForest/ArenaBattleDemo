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

	// �ִ� ü�� ���� �Լ�
	FORCEINLINE void SetMaxHP(float NewMaxHP) { MaxHP = NewMaxHP; }

	// HpBar�� �ۼ�Ʈ�� ������ �� ȣ���� �Լ�
	void UpdateHpBar(float NewCurrentHP);


protected:
	virtual void NativeConstruct() override;

protected:
	// HP �������� �����ֱ� ���� ����� ���α׷����� ���� ����
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	// ü�� ü�� �� // stat component���� �ٷ� �������°� �ƴ϶� ĳ���Ϳ��� �������� ������� �� ����
	UPROPERTY()
	float MaxHP;
};
