// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABWidgetComponent.h"
#include "UI/ABUserWidget.h"

void UABWidgetComponent::InitWidget()
{
	// InitWidget �Լ��� ȸ��Ǹ� WidgetClass ������ �������� ���� �ν��Ͻ��� ������
	// ���ο��� CreateWidget �Լ��� ����
	Super::InitWidget();

	// Super::InitWidget() �Լ� ȣ�� ���Ŀ��� ���� �ν��Ͻ��� �ִٰ� Ȯ���� �� ����
	// ������Ʈ�� ���� �Ѵٴ� ���� �̹� ��� ���� �ִ� ���� �� �� ���� �� �����ְ� �ִٶ�� �Ǵ��� ����
	// ��ȿ�� �˻��ϸ� �������� ���ɿ��� ������ �ʴ�
	UABUserWidget* ABUserWidget = Cast<UABUserWidget>(GetWidget());
	if (ABUserWidget)
	{
		ABUserWidget->SetOwningActor(GetOwner());
	}
}
