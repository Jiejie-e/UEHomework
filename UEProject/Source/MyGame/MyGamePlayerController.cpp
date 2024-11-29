// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyGamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "MyPlayerState.h"
#include "MyGameStateBase.h"
#include "MyHUD.h"
#include "Engine/LocalPlayer.h"

void AMyGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

}


