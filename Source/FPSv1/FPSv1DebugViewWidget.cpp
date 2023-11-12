// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSv1DebugViewWidget.h"
#include "MyCharacter.h"
#include "FPSv1Character.h"
#include "FPSv1DebugListItem.h"
#include "FPSv1DebugListObject.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ListView.h"
#include "Kismet/GameplayStatics.h"
#include "FPSv1PlayerController.h"

void UFPSv1DebugViewWidget::NativeConstruct()
{
	Super::NativeConstruct();
	DebugListView->SetListItems(TArray<UFPSv1DebugListObject*>());
	
	TargetPlayer = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//TargetPlayer = Cast<AFPSv1Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//ControlRotation = TargetPlayer->GetControlRotation();
	TargetFPCamera = TargetPlayer->GetFPCamera();
	TargetCameraBoom = TargetPlayer->GetCameraBoom();
}

void UFPSv1DebugViewWidget::RefreshListView()
{
	AFPSv1PlayerController* PlayerController = Cast<AFPSv1PlayerController>(TargetPlayer->GetController());

	AddToListCharacterData("Control Rotation", PlayerController->GetControlRotation().ToString());

	AddToListCharacterData(TargetPlayer->GetName() + "Location", TargetPlayer->GetActorLocation().ToString());
	AddToListCharacterData(TargetPlayer->GetName() + "Rotation", TargetPlayer->GetActorRotation().ToString());
	AddToListCharacterData("Aim Rotator", FRotator(TargetPlayer->GetPitch(), TargetPlayer->GetYaw(), 0.f).ToString());

	AddToListCharacterData(TargetFPCamera->GetName() + "Location", TargetFPCamera->GetRelativeLocation().ToString());
	AddToListCharacterData(TargetFPCamera->GetName() + "Rotation", TargetFPCamera->GetRelativeRotation().ToString());

	AddToListCharacterData(TargetCameraBoom->GetName() + "Location", TargetCameraBoom->GetRelativeLocation().ToString());
	AddToListCharacterData(TargetCameraBoom->GetName() + "Rotation", TargetCameraBoom->GetRelativeRotation().ToString());
	
	AddToListCharacterData(TEXT("BaseAimRotation"), TargetPlayer->GetBaseAimRotation().ToString());
}

void UFPSv1DebugViewWidget::AddToListCharacterData(FString _TargetName, FString _TargetContent)
{
	UFPSv1DebugListObject* DebugListObject = NewObject<UFPSv1DebugListObject>(this, UFPSv1DebugListObject::StaticClass());
	DebugListObject->TargetName = _TargetName;
	DebugListObject->TargetContents = _TargetContent;
	DebugListView->AddItem(DebugListObject);
}

void UFPSv1DebugViewWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	DebugListView->ClearListItems();
	RefreshListView();
}