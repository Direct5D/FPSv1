// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Array.h"
#include "Item.h"
#include "Weapon.h"
#include "Consumable.h"
#include "MyConstant.h"
#include "Inventory.generated.h"

DECLARE_DELEGATE_OneParam(FDeleSingleOneParam, int);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSV1_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventory();
	bool AddToInventory(class AItem* TargetItem, int32 weaponIdx);
	bool RemoveFromInventory(class AItem* TargetItem);
	void UpdateWeight();
	void ChageItemLocation();

	AConsumable* FindAmmoCanNull(AmmoType TargetAmmoType) const;
	TArray<class AItem*> GetInventoryArray() const { return InventoryArray; }
	class AWeapon** GetWeaponInventory() { return WeaponInventory; }

	FDeleSingleOneParam FuncDeleSingleOneParamWhenGetWeapon;
	FDeleSingleOneParam FuncDeleSingleOneParamWhenDropWeapon;
	FDeleSingleOneParam FuncDeleSingleOneParamWhenSwitchWeapon;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Contents")
	TArray<class AItem*> InventoryArray;
	UPROPERTY(VisibleAnywhere, Category = "Contents")
	class AWeapon* WeaponInventory[MAXIMUM_SLOT];
	UPROPERTY(VisibleAnywhere, Category = "Weight")
	float InventoryCapacity;
	UPROPERTY(VisibleAnywhere, Category = "Weight")
	float CurrentWeight;
};
