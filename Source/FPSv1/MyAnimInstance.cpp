// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAnimInstance.h"
#include "MyCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "FPSv1PlayerController.h"


void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CurrentPawn = TryGetPawnOwner();
	CurrentCharacter = Cast<AMyCharacter>(CurrentPawn);

	if (CurrentPawn == nullptr || CurrentCharacter == nullptr)
	{
		return;
	}

	CurrentSpeed = CurrentPawn->GetVelocity().Size();
	if (CurrentPawn->GetVelocity().X > 0 || CurrentPawn->GetVelocity().Y > 0)
	{
		IsMoving = true;
	}
	else
	{
		IsMoving = false;
	}
	
	IsDead = CurrentCharacter->GetIsDying();
	IsCrouching = CurrentCharacter->GetIsCrouching();
	IsCrawling = CurrentCharacter->GetIsProning();
	IsInAir = CurrentCharacter->GetMovementComponent()->IsFalling();

	HorizontalValue = CurrentCharacter->GetHorizontalValue();
	VerticalValue = CurrentCharacter->GetVerticalValue();

	Pitch = CurrentCharacter->GetPitch();
	Yaw = CurrentCharacter->GetYaw();

	CurrentWeapon = CurrentCharacter->GetCurrentWeaponSlot();
	LeftHandTargetLoc = CurrentCharacter->GetLeftHandTarget();


	IsTurningRight = CurrentCharacter->GetIsTurningRight();
	IsTurningLeft = CurrentCharacter->GetIsTurningLeft();
}

void UMyAnimInstance::AnimNotify_PosingStart()
{
	if (::IsValid(CurrentCharacter))
	{
		CurrentCharacter->SetCanSwitchingPose(false);
		UE_LOG(LogTemp, Warning, TEXT("UMyAnimInstance::AnimNotify_PosingStart"));
	}
}

void UMyAnimInstance::AnimNotify_PosingEnd()
{
	if (::IsValid(CurrentCharacter))
	{
		CurrentCharacter->SetCanSwitchingPose(true);
		UE_LOG(LogTemp, Warning, TEXT("UMyAnimInstance::AnimNotify_PosingEnd"));
	}
}
