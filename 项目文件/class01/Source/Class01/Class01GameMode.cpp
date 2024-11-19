// Copyright Epic Games, Inc. All Rights Reserved.

#include "Class01GameMode.h"
#include "Class01Character.h"
#include "UObject/ConstructorHelpers.h"

AClass01GameMode::AClass01GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
