// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSv1GameMode.generated.h"


UCLASS(minimalapi)
class AFPSv1GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPSv1GameMode();
	
	virtual void BeginPlay() override;
};
