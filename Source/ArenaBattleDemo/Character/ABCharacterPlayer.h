// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/ABCharacterHUDInterface.h"

#include "ABCharacterPlayer.generated.h"


/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API AABCharacterPlayer 
	: public AABCharacterBase, 
	public IABCharacterHUDInterface
{
	GENERATED_BODY()

public:

	AABCharacterPlayer();
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetCharacterControl(ECharacterControlType InNewCharacterControlType);
	virtual void SetCharacterControlData(const class UABCharacterControlData* InCharacterControlData) override;
protected:
	void ChangeCharacterControl();
	void ShoulderLook(const  FInputActionValue& Value);
	void ShoulderMove(const  FInputActionValue& Value);
	void QuarterMove(const  FInputActionValue& Value);
	void Attack();


	// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BLueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere, BLueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UCameraComponent> Camera;

	// Input Section
protected:
	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> AttackAction;
	
	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> QuarterMoveAction;

	// 현재 사용중인 모드
	ECharacterControlType CurrentCharacterControlType;
	
protected:
	// Inherited via IABCharacterHUDInterface
	virtual void SetUpHUDWidget(UABHUDWidget* InHUDWidget) override;

};
