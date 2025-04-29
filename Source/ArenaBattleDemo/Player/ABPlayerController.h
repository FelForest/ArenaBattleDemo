// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AABPlayerController();

	virtual void BeginPlay() override;
	
protected:
	// 생성할 위젯의 클래스 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UABHUDWidget> ABHUDwidgetClass;

	// 생성한 위젯을 객체 정보를 저장할 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UABHUDWidget> ABHUDWidget;
};
