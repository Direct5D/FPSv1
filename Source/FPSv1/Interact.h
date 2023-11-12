// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interact.generated.h"

UINTERFACE(MinimalAPI)
class UInteract : public UInterface
{
	GENERATED_BODY()
};

class FPSV1_API IInteract
{
	GENERATED_BODY()

public:
	virtual bool PickUp_Implementation(int32 weaponIdx = -1) { return true; };
	virtual bool Drop_Implementation() { return true; };
	virtual bool Detect_Implemenetation() { return true; }
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Interact_Implementation();
};
