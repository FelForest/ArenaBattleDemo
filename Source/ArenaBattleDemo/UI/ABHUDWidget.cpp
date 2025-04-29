// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHUDWidget.h"
#include "UI/ABHpBarWidget.h"
#include "UI/ABCharacterStatWidget.h"
#include "Interface/ABCharacterHUDInterface.h"

UABHUDWidget::UABHUDWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
}

void UABHUDWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	// HpBar의 최대 체력 스탯 설정.
	FABCharacterStat TotalStat = (BaseStat + ModifierStat);
	HpBar->SetMaxHP(TotalStat.MaxHp);

	// 캐릭터 스탯에 새로운 데이터 전달.
	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UABHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UABHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 이름으로 검색해 HPBar 위젯 설정
	HpBar = Cast<UABHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar != nullptr);

	// 이름으로 CharacterStat 위젯 설정
	CharacterStat = Cast<UABCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(CharacterStat != nullptr);
	
	IABCharacterHUDInterface* HUDPawn = Cast<IABCharacterHUDInterface>(GetOwningPlayer()->GetPawn());

	if (HUDPawn)
	{
		HUDPawn->SetUpHUDWidget(this);
	}
}
