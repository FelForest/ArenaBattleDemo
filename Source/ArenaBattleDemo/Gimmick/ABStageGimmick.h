// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABStageGimmick.generated.h"

UCLASS()
class ARENABATTLEDEMO_API AABStageGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABStageGimmick();

    // Stage Section.
protected:
    // 스테이지를 보여줄 때 사용할 메시 컴포넌트.
    UPROPERTY(VisibleAnywhere, Category = Stage, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UStaticMeshComponent> Stage;

    UPROPERTY(VisibleAnywhere, Category = Stage, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UBoxComponent> StageTrigger;

    // 스테이지 트리거의 오버랩 이벤트 확인을 위한 함수.
    void OnStageTriggerBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    // Gate Section (4개의 문의 대한 섹션).
protected:
    // 4개의 게이트(문)가 필요하기 때문에 Map으로 관리.
    UPROPERTY(VisibleAnywhere, Category = Gate, meta = (AllowPrivateAccess = "true"))
    TMap<FName, TObjectPtr<class UStaticMeshComponent>> Gates;

    // 각 문의 트리거를 배열로 관리.
    UPROPERTY(VisibleAnywhere, Category = Gate, meta = (AllowPrivateAccess = "true"))
    TArray<TObjectPtr<class UBoxComponent>> GateTriggers;

    // 게이트 트리거의 오버랩 이벤트 확인을 위한 함수.
    void OnGateTriggerBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
};
