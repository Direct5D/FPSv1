// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Weapon.h"
#include "Consumable.h"
#include "MyGameInstance.generated.h"


USTRUCT()
struct FMyCharacterData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FName Name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MaxHp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float CurrentHp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 CurrentAmmo;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MaximumAmmo;
};

USTRUCT()
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeaponMeshPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	WeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeaponDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AmmoType WeaponAmmoType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaximumAmmoNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TexturePath;
};

USTRUCT()
struct FConsumableData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ConsumableName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ConsumableMeshPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ConsumableType ConsumableType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AmmoType TypeOfAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaximumNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeightOfItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TexturePath;
};

UCLASS()
class FPSV1_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();

	FMyCharacterData* GetStatDataByName(FName Name) const;
	TArray<FWeaponData*> GetWeaponDataArray() const;
	TArray<FConsumableData*> GetConsumableDataArray() const;

private:
	UPROPERTY()
	UDataTable* MyStat;
	UPROPERTY()
	UDataTable* WeaponList;
	UPROPERTY()
	UDataTable* ConsumableList;
};
