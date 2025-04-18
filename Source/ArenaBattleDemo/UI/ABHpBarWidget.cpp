// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/ABCharacterWidgetInterface.h"

UABHpBarWidget::UABHpBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), MaxHP(-1.0f)
{
	// 일부러 유효하지 하지 않은 값을 구분하기 위해 값 설정
}

void UABHpBarWidget::UpdateHpBar(float NewCurrentHP)
{
	// MaxHP의 값이 유효한지 확인용 -> 제대로 설정됐는지 확인
	ensure(MaxHP > 0.0f);

	// 프로그래스바  위젯 업데이트
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHP / MaxHP);
	}
}

void UABHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 이 함수가 호출 되면, UI에 대한 대부분의 값이 초기화 되었다고 생각할 수 있음
	
	// 위젯 참조 설정을 위해 이름으로 검색
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	// 궁금한것데 이것이 제대로 참조가 안되면 계속 호출하는가?
	ensure(HpProgressBar);

	// 하고 싶은 것 : 캐릭터에 내 정보(위젯)를 전달
	// 강참조를 피하기 위해 인터페이스를 후회해 전달 (느슨한 결합)
	IABCharacterWidgetInterface* CharacterWidget = Cast<IABCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		// 인터페이스를 통해서 캐릭터에 내 정보 전달
		CharacterWidget->SetupCharacterWidget(this);
	}
}
