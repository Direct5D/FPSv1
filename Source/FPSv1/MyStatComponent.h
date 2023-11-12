// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathDelegate)
DECLARE_MULTICAST_DELEGATE(FOnHPChangeDelegate)
DECLARE_MULTICAST_DELEGATE(FOnAmmoChangeDelegate)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSV1_API UMyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMyStatComponent();

	void SetStat(const FName Name);
	FName GetName() const { return Name; }

	void SetNewHp(const float NewHp);
	float GetMaxHp() const { return MaxHp; }
	float GetHpRatio() const { return CurrentHp / (float)MaxHp; }
	float GetCurrentHp() const { return CurrentHp; }

	void SetNewWeapon(const int32 NewMaximumAmmo, const int32 NewAmmo);
	void SetCurrentAmmo(const int32 NewAmmo);
	int32 GetCurrentAmmo() const { return CurrentAmmo; }
	int32 GetMaximumAmmo() const { return MaximumAmmo; }

	FOnDeathDelegate OnDeath;
	FOnHPChangeDelegate OnHPChanged;
	FOnAmmoChangeDelegate OnAmmoChanged;

private:
	UPROPERTY(VisibleAnywhere, Category = Stat)
	FName Name;
	UPROPERTY(VisibleAnywhere, Category = Stat)
	float MaxHp;
	UPROPERTY(VisibleAnywhere, Category = Stat)
	float CurrentHp;
	UPROPERTY(VisibleAnywhere, Category = Stat)
	int32 CurrentAmmo;
	UPROPERTY(VisibleAnywhere, Category = Stat)
	int32 MaximumAmmo;
};
