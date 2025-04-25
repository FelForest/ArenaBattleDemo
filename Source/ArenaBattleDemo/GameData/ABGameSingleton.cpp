// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/ABGameSingleton.h"

DEFINE_LOG_CATEGORY(LogABGameSingleton);

UABGameSingleton::UABGameSingleton()
{
	// 데이터 테이블 애셋 로드
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/ArenaBattle/GameData/ABCharacterStatTable.ABCharacterStatTable"));
	if (DataTableRef.Object != nullptr)
	{
		// UDataTable Key/Value로 관리됨
		// Map으로 사용하지 않고, 배열로 직접 관리
		const UDataTable* DataTable = DataTableRef.Object;
		// 데이터 테이블이 제대로 초기화됐는지 확인(검증)
		check(DataTable->GetRowMap().Num() > 0);

		// Value 값만 저장하기 위한 배열
		TArray<uint8*> ValueArray;

		// 데이터 테이블의 GenerateValueArray 함수를 사용해 Value 값만 배열에 저장
		// 동적 배열 할당 방지하는 방법 -> 사용하는 순간 들어올 데이터의 크기를 확인해서 처음부터 그 크기로 만들어 버리면 됨
		DataTable->GetRowMap().GenerateValueArray(ValueArray);

		// 배열 순환하면서 데이터
		//for (uint8* Value : ValueArray)
		//{
		//    CharacterStatTable.Add(*reinterpret_cast<FABCharacterStat*>(Value));
		//}
		// 이건 같은일을 한다
		// 알고리즘을 활용해 Value 값을 변환해 저장 
		Algo::Transform(ValueArray, CharacterStatTable, 
			[](uint8* Value)
			{
				return *reinterpret_cast<FABCharacterStat*>(Value);
			}
		);
	}

	// 최대 레벨 값 설정
	CharacterMaxLevel = CharacterStatTable.Num();

	// 값에 문제 없는지 확인
	ensure(CharacterMaxLevel > 0);


}

UABGameSingleton& UABGameSingleton::Get()
{
	// 싱글톤 가져오기(일반적으로는 말이 안됨)
	// 프로젝트 세팅에 싱글톤으로 지정했기 때문에 가능
	UABGameSingleton* Singleton = Cast<UABGameSingleton>(GEngine->GameSingleton);

	if (Singleton != nullptr)
	{
		return *Singleton;
	}

	// 도달 하면 안됨
	// 오류 출력
	UE_LOG(LogABGameSingleton, Error, TEXT("Invalid Game Singleton"));

	return *NewObject<UABGameSingleton>();
}
