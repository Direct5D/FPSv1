// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSv1PlayerController.h"
#include "FPSv1DebugViewWidget.h"
#include "Blueprint/UserWidget.h"


AFPSv1PlayerController::AFPSv1PlayerController()
{
	static ConstructorHelpers::FClassFinder<UFPSv1DebugViewWidget> DBW(TEXT("WidgetBlueprint'/Game/ThirdPersonCPP/Blueprints/BP_DebugViewWidget.BP_DebugViewWidget_C'"));
	if (DBW.Succeeded())
	{
		DebugWidgetClass = DBW.Class;
	}
	DebugWidget = Cast<UFPSv1DebugViewWidget>(CreateWidget(GetWorld(), DebugWidgetClass));
	

	static ConstructorHelpers::FClassFinder<UUserWidget> PW(TEXT("WidgetBlueprint'/Game/UI/BPW_PauseWidget.BPW_PauseWidget_C'"));
	if (PW.Succeeded())
	{
		PauseWidgetClass = PW.Class;
	}
	PauseWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), PauseWidgetClass));
	
}

void AFPSv1PlayerController::BeginPlay()
{
	Super::BeginPlay();
	InputPitchScale = 1.0f;
	InputYawScale = 1.0f;

	DebugWidget->AddToViewport();
	DebugWidget->SetVisibility(ESlateVisibility::Collapsed);

	PauseWidget->AddToViewport();
	PauseWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AFPSv1PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("ToggleDebugView", IE_Pressed, this, &AFPSv1PlayerController::OnOffDebugViewOnScreen);
	InputComponent->BindAction("Setting", IE_Pressed, this, &AFPSv1PlayerController::OpenSetting);
}

void AFPSv1PlayerController::OnOffDebugViewOnScreen()
{
	if (DebugWidget)
	{
		if (DebugWidget->GetVisibility() == ESlateVisibility::Hidden)
			DebugWidget->SetVisibility(ESlateVisibility::Visible);
		else if(DebugWidget->GetVisibility() == ESlateVisibility::Visible)
			DebugWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

FVector AFPSv1PlayerController::GetAdjustedAimDirection()
{
	FVector AimDirection;
	FVector CameraLocation;
	FVector CameraForward;

	FVector2D ScreenPosition;
	int32 a = 0, b = 0;
	GetViewportSize(a, b);
	
	FVector2D CrosshairScreenPosition = FVector2D(a * 0.5f, b * 0.5f);
	FVector WorldPosition;
	FVector WorldDirection;
	if (DeprojectScreenPositionToWorld(CrosshairScreenPosition.X, CrosshairScreenPosition.Y, WorldPosition, WorldDirection))
	{
		AimDirection = WorldDirection;
	}
	
	return AimDirection;
}

void AFPSv1PlayerController::OpenSetting()
{
	if (PauseWidget)
	{
			

		if (PauseWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			PauseWidget->SetVisibility(ESlateVisibility::Visible);
			SetInputMode(FInputModeUIOnly());
			SetShowMouseCursor(true);
		}	
		else if(PauseWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			PauseWidget->SetVisibility(ESlateVisibility::Collapsed);
			SetInputMode(FInputModeGameOnly());
			SetShowMouseCursor(false);
		}
	}
}