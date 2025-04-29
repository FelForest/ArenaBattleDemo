// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHUDWidget.h"

UABHUDWidget::UABHUDWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
}

void UABHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 여기에 스탯 컴포넌트에 있는 데이터들을 불러와 HUD에 배치된 테스트에 설정
	// 그리고 두 위젯과 연동 되로록 스탯 데이터가 변경되면 자동으로 처리하는 구조를 설계할 예정
}
