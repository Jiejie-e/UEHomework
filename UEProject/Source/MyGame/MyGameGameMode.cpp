// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyGameGameMode.h"
#include "MyGameCharacter.h"
#include "MyGamePlayerController.h"
#include "MyPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"

AMyGameGameMode::AMyGameGameMode()
	: Super()
{
	// 设置默认 Pawn 类
	DefaultPawnClass = AMyGameCharacter::StaticClass();
	// 设置默认 PlayerController 类
	PlayerControllerClass = AMyGamePlayerController::StaticClass();

	PlayerStateClass = AMyPlayerState::StaticClass();

	bReplicates = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;


}

