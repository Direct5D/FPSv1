// Fill out your copyright notice in the Description page of Project Settings.


#include "CapturePlayerAnimInstance.h"
#include "CapturePlayer.h"

void UCapturePlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//CurrentPawn = TryGetPawnOwner();
	CurrentCharacter = Cast<ACapturePlayer>(TryGetPawnOwner());

	if (CurrentCharacter == nullptr)
	{
		return;
	}

	CurrentWeapon = CurrentCharacter->GetCurrentWeaponSlot();
}

