// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterStatWidget.h"
#include "Components/TextBlock.h"

void UABCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 리플렉션 기능을 활용

	// FABCharacterStat 구조체가 가지는 속성을 가져와서 설정
	for (
		TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct());
		PropIt;
		++PropIt)
	{
		// 속성의 키값(이름 값)
		const FName PropKey(PropIt->GetName());

		// Base 스탯에 대한 텍스트 블록 이름
		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"), *PropIt->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("Txt%sBase"), *PropIt->GetName());

		// Modifier 스탯에 대한 텍스트 블록 이름
		const FName TextModifierControlName = *FString::Printf(TEXT("Txt%sModifier"), *PropIt->GetName());

		// 이름 값을 사용해 텍스트 블록 로드
		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName));
		UE_LOG(LogTemp, Warning, TEXT("%s"), *PropKey.ToString());
		UE_LOG(LogTemp, Warning, TEXT("%p"), BaseTextBlock);

		// 제대로 찾았으면
		if (BaseTextBlock)
		{
			BaseLookup.Add(PropKey, BaseTextBlock);
		}

		// 이름 값을 사용해 텍스트 블록 로드
		UTextBlock* ModifierTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextModifierControlName));

		// 제대로 찾았으면
		if (ModifierTextBlock)
		{
			ModifierLookup.Add(PropKey, ModifierTextBlock);
		}
	}
}

void UABCharacterStatWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	// FABCharacterStat 구조체가 가지는 속성을 가져와서 설정
	for (
		TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct());
		PropIt;
		++PropIt)
	{
		// 속성의 키값(이름 값)
		const FName PropKey(PropIt->GetName());

		// Base 스탯 데이터 설정을 위한 변수
		float BaseData = 0.0f;

		PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseData);

		// Modifier 스탯 데이터 설정을 위한 변수
		float ModifierData = 0.0f;

		PropIt->GetValue_InContainer(reinterpret_cast<const void*>(&ModifierStat), &ModifierData);

		// 맵에서 Base 텍스트 블록을 가져와 값 설정
		UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey);
		if (BaseTextBlockPtr) // 이것도 결국 포인터이다
		{
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData)));
		}

		// 맵에서 Modifier 텍스트 블록을 가져와 값 설정
		UTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(PropKey);
		if (ModifierTextBlockPtr) // 이것도 결국 포인터이다
		{
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierData)));
		}
	}
}
