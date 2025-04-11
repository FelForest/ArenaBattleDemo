// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/ABFountain.h"
//#include "Components/StaticMeshComponent.h" // ???

// Sets default values
AABFountain::AABFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water"));

	// 기본 컴포넌트를 BOdy 컴포넌트로 설정
	RootComponent = Body;

	// Water를 Body 하위 컴포넌트로 설정
	Water->SetupAttachment(Body);
	// Water를 상대 위치로 알맞게 설정
	Water->SetRelativeLocation(FVector(0, 0, 135.0f));

	// 에셋 검색
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FountainBodyMesh(TEXT("/Game/ArenaBattle/Environment/Props/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01"));

	if (FountainBodyMesh.Object != nullptr)
	{
		// 리소스 설정
		Body->SetStaticMesh(FountainBodyMesh.Object);
	}
	
	// 에셋 검색
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FountainWaterMesh(TEXT("/Game/ArenaBattle/Environment/Props/SM_Plains_Fountain_02.SM_Plains_Fountain_02"));

	if (FountainWaterMesh.Object != nullptr)
	{
		// 리소스 설정
		Water->SetStaticMesh(FountainWaterMesh.Object);
	}
	
}

// Called when the game starts or when spawned
void AABFountain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

