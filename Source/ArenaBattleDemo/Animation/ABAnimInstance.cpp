// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ABAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UABAnimInstance::UABAnimInstance()
	: MovingThreshold(3.0f), JumpingThreshold(100.0f)
{
	// 문턱 값 설정
}

void UABAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 애님 인스턴스를 소유한 폰(액터)를 캐릭터로 변환
	Owner = Cast<ACharacter>(GetOwningActor());

	// 캐릭터로 형변환 성공
	if (Owner != nullptr)
	{
		// 무브먼트 컴포넌트 검색
		Movement = Owner->GetCharacterMovement();
	}
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 필요한 값 업데이트
	if (Movement != nullptr)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshold;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
	}
}
