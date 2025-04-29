// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"
#include "UI/ABHUDWidget.h"

AABPlayerController::AABPlayerController()
{
	// 위젯 블루프린트 에셋에서 클래스 정보 로드
	static ConstructorHelpers::FClassFinder<UABHUDWidget> ABHUDWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_ABHUD.WBP_ABHUD_C"));

	if (ABHUDWidgetRef.Class != nullptr)
	{
		ABHUDwidgetClass = ABHUDWidgetRef.Class;
	}
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 시작 때 마우스가 뷰포트로 입력되어 바로 실행되도록 설정
	FInputModeGameOnly GameInput;
	SetInputMode(GameInput);

	// 이것 보장해줘야 하는거 아닌가
	// 위젯 생성
	ABHUDWidget = CreateWidget<UABHUDWidget>(this, ABHUDwidgetClass);

	// 위젯이 문제 없이 생성 됐으면
	if (ABHUDWidget)
	{
		// 위젯을 화면에 추가해 UI가 보일 수 있도록 설정
		ABHUDWidget->AddToViewport();
	}
}
