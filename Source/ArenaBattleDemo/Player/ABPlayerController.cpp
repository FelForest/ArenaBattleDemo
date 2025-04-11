// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 시작 때 마우스가 뷰포트로 입력되어 바로 실행되도록 설정
	FInputModeGameOnly GameInput;
	SetInputMode(GameInput);
}
