// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "InputActionValue.h"

#include "ABCharacterPlayer.generated.h"


/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API AABCharacterPlayer : public AABCharacterBase
{
	GENERATED_BODY()

public:

	AABCharacterPlayer();
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void Move(const  FInputActionValue& Value);
	void Look(const  FInputActionValue& Value);
	
	// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BLueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere, BLueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UCameraComponent> Camera;

	// Input Section
protected:
	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "ture"))
	TObjectPtr<class UInputAction> LookAction;
	
};
