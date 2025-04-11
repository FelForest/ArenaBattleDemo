// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "InputMappingContext.h"
#include "EnhancedInputCOmponent.h"
#include "EnhancedInputSubsystems.h"

#include "GameFramework/CharacterMovementComponent.h"
AABCharacterPlayer::AABCharacterPlayer()
{
	// 컨트롤러의 회전을 받아서 설정하는 모드를 모두 해제
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 무드번트 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 800.0f;
	
	// 컴포넌트 설정
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f,0.0f,-88.0f), FRotator(0.0f, -90.0f, 0.0f));
	// 리소스 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"));

	// 스캘레탈 메시 설정
	if (CharacterMesh.Object != nullptr)
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}

	// 애니메이션 인스턴스 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> CharacterAnim(TEXT("/Game/Characters/Mannequins/Animations/ABP_Manny.ABP_Manny_C"));
	if (CharacterAnim.Class != nullptr)
	{
		GetMesh()->SetAnimClass((CharacterAnim.Class));
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bUsePawnControlRotation = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef = TEXT("/Game/ArenaBattle/Input/IMC_Default.IMC_Default");
	if (InputMappingContextRef.Object != nullptr)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpInputActionRef = TEXT("/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump");
	if (JumpInputActionRef.Object != nullptr)
	{
		JumpAction = JumpInputActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveInputActionRef = TEXT("/Game/ArenaBattle/Input/Actions/IA_Move.IA_Move");
	if (MoveInputActionRef.Object != nullptr)
	{
		MoveAction = MoveInputActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookInputActionRef = TEXT("/Game/ArenaBattle/Input/Actions/IA_Look.IA_Look");
	if (LookInputActionRef.Object != nullptr)
	{
		LookAction = LookInputActionRef.Object;
	}
}

void AABCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	// CastChecked 오류 발생 시킴
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());

	if (PlayerController != nullptr)
	{
		if (auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AABCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto EnhancedInputCompoenent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//BInding
	EnhancedInputCompoenent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputCompoenent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Move);
	EnhancedInputCompoenent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Look);
}

void AABCharacterPlayer::Move(const FInputActionValue& Value)
{
	// 입력값 읽기
	FVector2D Movement = Value.Get<FVector2D>();

	// 컨트롤러의 회전값
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f,Rotation.Yaw ,0.0f);

	// 방향 구하기
	FVector ForwardVector =FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RIghtVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(ForwardVector, Movement.X);
	AddMovementInput(RIghtVector, Movement.Y);
}

void AABCharacterPlayer::Look(const FInputActionValue& Value)
{
	// 입력 값 읽기
	FVector2D LookVector = Value.Get<FVector2D>();

	// 컨트롤러에 회전 추가
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}
