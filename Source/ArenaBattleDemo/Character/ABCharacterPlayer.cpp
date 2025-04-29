// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "InputMappingContext.h"
#include "EnhancedInputCOmponent.h"
#include "EnhancedInputSubsystems.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Character/ABCharacterControlData.h"
#include "UI/ABHUDWidget.h"
#include "CharacterStat/ABCharacterStatComponent.h"

AABCharacterPlayer::AABCharacterPlayer()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bUsePawnControlRotation = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpInputActionRef = TEXT("/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump");
	if (JumpInputActionRef.Object != nullptr)
	{
		JumpAction = JumpInputActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ShoulderMoveInputActionRef = TEXT("/Game/ArenaBattle/Input/Actions/IA_ShoulderMove.IA_ShoulderMove");
	if (ShoulderMoveInputActionRef.Object != nullptr)
	{
		ShoulderMoveAction = ShoulderMoveInputActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ShoulderLookInputActionRef = TEXT("/Game/ArenaBattle/Input/Actions/IA_ShoulderLook.IA_ShoulderLook");
	if (ShoulderLookInputActionRef.Object != nullptr)
	{
		ShoulderLookAction = ShoulderLookInputActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> QuarterMoveInputActionRef = TEXT("/Game/ArenaBattle/Input/Actions/IA_QuarterMove.IA_QuarterMove");
	if (QuarterMoveInputActionRef.Object != nullptr)
	{
		QuarterMoveAction = QuarterMoveInputActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ChangeControlActionRef = TEXT("/Game/ArenaBattle/Input/Actions/IA_ChangedControl.IA_ChangedControl");
	if (ChangeControlActionRef.Object != nullptr)
	{
		ChangeControlAction = ChangeControlActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef = TEXT("/Game/ArenaBattle/Input/Actions/IA_Attack.IA_Attack");
	if (AttackActionRef.Object != nullptr)
	{
		AttackAction = AttackActionRef.Object;
	}

	// 초기 설정
	CurrentCharacterControlType = ECharacterControlType::Quarter;
}

void AABCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		// 입력 활성화
		EnableInput(PlayerController);
	}

	// 입력 설정
	SetCharacterControl(CurrentCharacterControlType);
}

void AABCharacterPlayer::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		// 입력 비활성화
		DisableInput(PlayerController);
	}
}

void AABCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto EnhancedInputCompoenent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//BInding
	EnhancedInputCompoenent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputCompoenent->BindAction(ChangeControlAction, ETriggerEvent::Started, this, &AABCharacterPlayer::ChangeCharacterControl);
	EnhancedInputCompoenent->BindAction(AttackAction, ETriggerEvent::Started, this, &AABCharacterPlayer::Attack);
	EnhancedInputCompoenent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderMove);
	EnhancedInputCompoenent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderLook);
	EnhancedInputCompoenent->BindAction(QuarterMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::QuarterMove);
}

void AABCharacterPlayer::SetCharacterControl(ECharacterControlType InNewCharacterControlType)
{
	// 변경할 컨트롤 타입에 대응하는 데이터 애셋 로드(TMap으로부터)
	UABCharacterControlData* NewCharacterControl = CharacterControlManager[InNewCharacterControlType];
	check(NewCharacterControl != nullptr);

	// 데이터 애셋을 사용해 관련 값 설정
	SetCharacterControlData(NewCharacterControl);

	// CastChecked 오류 발생 시킴
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());

	if (PlayerController != nullptr)
	{
		if (auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			//SubSystem->RemoveMappingContext(); // 이걸로 해봐라 여러 타입이면 이게 맞는거 같음
			SubSystem->ClearAllMappings();
			SubSystem->AddMappingContext(NewCharacterControl->InputMappingContext, 0);
		}
	}

	// 현재 사용 중인 캐릭터 컨트롤 타입 업데이트.
	CurrentCharacterControlType = InNewCharacterControlType;
}

void AABCharacterPlayer::SetCharacterControlData(const class UABCharacterControlData* InCharacterControlData)
{
	Super::SetCharacterControlData(InCharacterControlData);

	// SpringArm 관련 설정.
	SpringArm->TargetArmLength = InCharacterControlData->TargetArmLength;
	SpringArm->SetRelativeRotation(InCharacterControlData->RelativeRotation);
	SpringArm->bUsePawnControlRotation = InCharacterControlData->bUsePawnControlRotation;
	SpringArm->bInheritPitch = InCharacterControlData->bInheritPitch;
	SpringArm->bInheritYaw = InCharacterControlData->bInheritYaw;
	SpringArm->bInheritRoll = InCharacterControlData->bInheritRoll;
	SpringArm->bDoCollisionTest = InCharacterControlData->bDoCollisionTest;
}

void AABCharacterPlayer::ChangeCharacterControl()
{
	// 사용할 캐릭터 컨트롤을 변경하는 함수
	if (CurrentCharacterControlType == ECharacterControlType::Quarter)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quarter);
	}
}

void AABCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
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

void AABCharacterPlayer::QuarterMove(const FInputActionValue& Value)
{
	// 입력 값 읽기.
	FVector2D Movement = Value.Get<FVector2D>();

	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = Movement.SizeSquared();

	// 두 방향으로 입력이 들어오면, 이동 방향은 정규화해 크기를 1로 만들고,
	// 입력 스케일을 1로 강제 설정.
	if (MovementVectorSizeSquared > 1.0f)
	{
		Movement.Normalize();
		MovementVectorSize = 1.0f;
	}
	// 입력이 1이하이면, 해당 입력을 스케일로 사용하기 위해 값 계산.
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(Movement.X, Movement.Y, 0.0f);

	// 캐릭터가 이동하는 방향에 맞게 컨트롤러 회전 설정
	Controller->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	
	// 입력에 따른 방향으로 이동하도록 입력 전달.
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AABCharacterPlayer::Attack()
{
	// 공격 입력 처리 함수 호출
	ProcessComboCommand();
}

void AABCharacterPlayer::SetUpHUDWidget(UABHUDWidget* InHUDWidget)
{
	if (InHUDWidget == nullptr)
	{
		return;
	}

	// 스탯 정보를 UI에 전달
	InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());

	// HP 정보 전달
	InHUDWidget->UpdateHpBar(Stat->GetCurrentHP());

	UE_LOG(LogTemp, Warning, TEXT("세팅됨"));
	// 델리게이트에 등록.
	Stat->OnStatChanged.AddUObject(InHUDWidget, &UABHUDWidget::UpdateStat);
	Stat->OnHpChanged.AddUObject(InHUDWidget, &UABHUDWidget::UpdateHpBar);
}


void AABCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	// 입력 값 읽기
	FVector2D LookVector = Value.Get<FVector2D>();

	// 컨트롤러에 회전 추가
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}
