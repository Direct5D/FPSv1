// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "Kismet/GameplayStatics.h"


UInventory::UInventory()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	InventoryCapacity = 100.f;
	CurrentWeight = 0.f;

}

bool UInventory::AddToInventory(AItem* TargetItem, int32 weaponIdx)
{
	if (TargetItem->IsA(AWeapon::StaticClass()))
	{
		AWeapon* TargetWeapon = Cast<AWeapon>(TargetItem);
		WeaponType TargetWeaponType = TargetWeapon->GetTypeOfWeapon();
		int EquipedSlotNum = -1;
		if (weaponIdx != -1)
		{
			if (TargetWeaponType == WeaponType::MainWeapon)
			{
				if (weaponIdx == FIRST_SLOT && WeaponInventory[FIRST_SLOT] == nullptr)
				{
					WeaponInventory[FIRST_SLOT] = TargetWeapon;
					EquipedSlotNum = FIRST_SLOT;
				}
				else if (weaponIdx == SECOND_SLOT && WeaponInventory[SECOND_SLOT] == nullptr)
				{
					WeaponInventory[SECOND_SLOT] = TargetWeapon;
					EquipedSlotNum = SECOND_SLOT;
				}
				else
				{
					return false;
				}
			}
			else if (TargetWeaponType == WeaponType::SecondaryWeapon && weaponIdx == THIRD_SLOT && WeaponInventory[THIRD_SLOT] == nullptr)
			{
				WeaponInventory[THIRD_SLOT] = TargetWeapon;
				EquipedSlotNum = THIRD_SLOT;
			}
			else
			{
				return false;
			}
		}
		else
		{
			switch (TargetWeaponType)
			{
			case WeaponType::MainWeapon:
				if (WeaponInventory[FIRST_SLOT] == nullptr)
				{
					WeaponInventory[FIRST_SLOT] = TargetWeapon;
					EquipedSlotNum = FIRST_SLOT;
				}
				else if (WeaponInventory[SECOND_SLOT] == nullptr)
				{
					WeaponInventory[SECOND_SLOT] = TargetWeapon;
					EquipedSlotNum = SECOND_SLOT;
				}
				break;
			case WeaponType::SecondaryWeapon:
				if (WeaponInventory[THIRD_SLOT] == nullptr)
				{
					WeaponInventory[THIRD_SLOT] = TargetWeapon;
					EquipedSlotNum = THIRD_SLOT;
				}
				break;
			case WeaponType::Melee:
				break;
			default:
				check(false);
				break;
			}
		}
		
		if (EquipedSlotNum != -1)	// equip
		{
			FuncDeleSingleOneParamWhenGetWeapon.ExecuteIfBound(EquipedSlotNum);
			return true;
		}
		else	// can not equip
		{
			return false;
		}
	}

	if (TargetItem->IsA(AConsumable::StaticClass()))
	{
		AConsumable* TargetConsumable = Cast<AConsumable>(TargetItem);
		TargetConsumable->OnChangedItem.AddUObject(this, &UInventory::UpdateWeight);

		float Weight = TargetConsumable->GetWeightOfItem();
		if (CurrentWeight + Weight < InventoryCapacity)
		{
			InventoryArray.Add(TargetConsumable);
			UpdateWeight();
			return true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString("Inventory Full"));
		}
	}

	return false;
}

bool UInventory::RemoveFromInventory(AItem* TargetItem)
{
	UE_LOG(LogTemp, Warning, TEXT("UInventory::RemoveFromInventory"));
	if (TargetItem->IsA(AConsumable::StaticClass()))
	{
		
		return InventoryArray.Remove(TargetItem) == 1 ? true : false;
	}
	if (TargetItem->IsA(AWeapon::StaticClass()))
	{
		AWeapon* TargetWeapon = Cast<AWeapon>(TargetItem);
		//WeaponType TargetWeaponType = TargetWeapon->GetTypeOfWeapon();
		for (int i = 0; i < MAXIMUM_SLOT; i++)
		{
			if (WeaponInventory[i] == TargetWeapon)
			{
				FuncDeleSingleOneParamWhenDropWeapon.ExecuteIfBound(i);
				WeaponInventory[i] = nullptr;
				// un equip?

				return true;
			}
		}
	}
	return false;
}

void UInventory::UpdateWeight()
{
	float NewCurrentWeight = 0;
	for (AItem* item : InventoryArray)
	{
		if (item->IsA(AConsumable::StaticClass()))
		{
			AConsumable* ItemWithWeight = Cast<AConsumable>(item);
			NewCurrentWeight += ItemWithWeight->GetWeightOfItem();
		}
	}
	CurrentWeight = NewCurrentWeight;
}

AConsumable* UInventory::FindAmmoCanNull(AmmoType TargetAmmoType) const
{
	for (AItem* item : InventoryArray)
	{
		if (item->IsA(AConsumable::StaticClass()))
		{
			AConsumable* ConsumableItem = Cast<AConsumable>(item);
			if (ConsumableItem->GetItemType() == ConsumableType::Ammo && ConsumableItem->GetAmmoType() == TargetAmmoType)
			{
				return ConsumableItem;
			}
		}
	}
	return nullptr;
}

void UInventory::ChageItemLocation()
{
	// AConsumable not yet

	//weapon slot 1 <-> slot 2
	if (WeaponInventory[FIRST_SLOT] && WeaponInventory[SECOND_SLOT])
	{
		AWeapon* tmp = WeaponInventory[FIRST_SLOT];
		WeaponInventory[FIRST_SLOT] = WeaponInventory[SECOND_SLOT];
		WeaponInventory[SECOND_SLOT] = tmp;
	}
	else
	{
		if (WeaponInventory[FIRST_SLOT])
		{
			WeaponInventory[SECOND_SLOT] = WeaponInventory[FIRST_SLOT];
			WeaponInventory[FIRST_SLOT] = nullptr;
		}
		else
		{
			WeaponInventory[FIRST_SLOT] = WeaponInventory[SECOND_SLOT];
			WeaponInventory[SECOND_SLOT] = nullptr;
		}
	}
	FuncDeleSingleOneParamWhenSwitchWeapon.ExecuteIfBound(MAXIMUM_SLOT);
}