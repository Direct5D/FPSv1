// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSv1GameMode.h"
#include "FPSv1Character.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/HUD.h"

AFPSv1GameMode::AFPSv1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	static ConstructorHelpers::FClassFinder<AHUD> HUDBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_FPSv1HUD"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		HUDClass = HUDBPClass.Class;
	}
}
