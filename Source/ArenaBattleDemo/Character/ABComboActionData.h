// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API UABComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UABComboActionData();

public:

	// 몽타주 섹션 이름 접두사 (ComboAttack)
	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	// 최대 콤보 개수 (4개)
	UPROPERTY(EditAnywhere, Category = ComboData)
	uint8 MaxComboCount;

	// 프레임 재생 속도
	// 애니메이션 애셋의 기본 재생 속도 값이 설정됨
	UPROPERTY(EditAnywhere, Category = ComboData)
	float FrameRate;

	// 입력이 사전에 들어왔는지를 확인하는 프레임.
	// 공격 애니메이션 에셋 별로 지정
	UPROPERTY(EditAnywhere, Category = ComboaData)
	TArray<float> EffectiveFrameCount;
};
