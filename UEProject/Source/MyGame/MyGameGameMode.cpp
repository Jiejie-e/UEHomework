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
	// ����Ĭ�� Pawn ��
	DefaultPawnClass = AMyGameCharacter::StaticClass();
	// ����Ĭ�� PlayerController ��
	PlayerControllerClass = AMyGamePlayerController::StaticClass();

	PlayerStateClass = AMyPlayerState::StaticClass();

	bReplicates = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;


}

