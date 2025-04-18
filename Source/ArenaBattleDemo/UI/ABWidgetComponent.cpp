// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABWidgetComponent.h"
#include "UI/ABUserWidget.h"

void UABWidgetComponent::InitWidget()
{
	// InitWidget 함수가 회출되면 WidgetClass 정보를 바탕으로 위젯 인스턴스를 생성함
	// 내부에서 CreateWidget 함수를 실행
	Super::InitWidget();

	// Super::InitWidget() 함수 호출 이후에는 위젯 인스턴스가 있다고 확인할 수 있음
	// 컴포넌트가 실행 한다는 것은 이미 어디에 속해 있는 것을 알 수 있음 즉 소유주가 있다라고 판단이 가능
	// 유효성 검사하면 좋다지만 성능에서 좋지는 않다
	UABUserWidget* ABUserWidget = Cast<UABUserWidget>(GetWidget());
	if (ABUserWidget)
	{
		ABUserWidget->SetOwningActor(GetOwner());
	}
}
