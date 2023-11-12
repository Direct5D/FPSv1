// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CapturePlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FPSV1_API UCapturePlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	//APawn* CurrentPawn;
	class ACapturePlayer* CurrentCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	int32 CurrentWeapon;
};
