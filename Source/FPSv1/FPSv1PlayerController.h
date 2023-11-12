// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSv1PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FPSV1_API AFPSv1PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFPSv1PlayerController();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void OnOffDebugViewOnScreen();
	FVector GetAdjustedAimDirection();
	void OpenSetting();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UUserWidget> DebugWidgetClass;
	UPROPERTY()
	class UFPSv1DebugViewWidget* DebugWidget;
	UPROPERTY()
	TSubclassOf<class UUserWidget> PauseWidgetClass;
	class UUserWidget* PauseWidget;
};
