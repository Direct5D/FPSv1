// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSv1HUD.generated.h"

/**
 * 
 */
UCLASS()
class FPSV1_API AFPSv1HUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	UTexture2D* CrosshairTexture;

public:
	virtual void DrawHUD() override;

};
