#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ABCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FABCharacterStat : public FTableRowBase
{
    GENERATED_BODY()

public:
    FABCharacterStat()
        : MaxHp(0.0f), Attack(0.0f), AttackRange(0.0f), AttackSpeed(0.0f), MovementSpeed(0.0f)
    {
    }

    // 연산자 오버로딩.
    FABCharacterStat operator+(const FABCharacterStat& Other) const
    {
        // 현재 데이터 포인터.
        const float* const ThisPtr = reinterpret_cast<const float* const>(this);

        // 덧셈할 데이터 포인터.
        const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

        // 결과 저장할 구조에.
        FABCharacterStat Result;

        // 결과 구조체 포인터.
        float* ResultPtr = reinterpret_cast<float*>(&Result);

        // 구조체에 포함된 float 변수의 개수 확인.
        int32 StatNum = sizeof(FABCharacterStat) / sizeof(float);

        // 포인터를 활용한 덧셈 처리.
        for (int32 ix = 0; ix < StatNum; ++ix)
        {
            ResultPtr[ix] = ThisPtr[ix] + OtherPtr[ix];
        }

        // 결과 반환.
        return Result;
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
    float MaxHp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
    float Attack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
    float AttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
    float AttackSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
    float MovementSpeed;

    
};