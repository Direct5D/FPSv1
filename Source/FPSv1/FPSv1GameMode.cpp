// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSv1GameMode.h"
#include "FPSv1Character.h"
#include "UObject/ConstructorHelpers.h"

AFPSv1GameMode::AFPSv1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
