// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"

AABGameMode::AABGameMode()
{
	/*ConstructorHelpers::FClassFinder<APawn> DefaultCharacterRef(
		TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));*/
	ConstructorHelpers::FClassFinder<APawn> DefaultCharacterRef(
		TEXT("/Script/ArenaBattleDemo.ABCharacterPlayer"));

	if (DefaultCharacterRef.Class)
	{
		DefaultPawnClass = DefaultCharacterRef.Class;
	}

	// 이렇게 하면 뭐가 좋냐 -> include를 안 쓸 수 있음 야호!
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(
		TEXT("/Script/ArenaBattleDemo.ABPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
	
	//PlayerControllerClass = AABPlayerController::StaticClass();
}
