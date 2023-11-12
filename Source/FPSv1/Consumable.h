// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Consumable.generated.h"

UENUM(Blueprintable)
enum class ConsumableType : uint8
{
	Ammo = 0,
	Attachment = 1,
	Doping = 2,
};

DECLARE_MULTICAST_DELEGATE(FOnChangedItemDelegate)

UCLASS()
class FPSV1_API AConsumable : public AItem
{
	GENERATED_BODY()
public:
	AConsumable();

	void LoadConsumableData(FName ConsumableName, FString ConsumableMeshPath, ConsumableType ConsumableType, AmmoType Ammo, int32 MaximumNum, int32 CurrentNum, float WeightOfItem, FString TexturePath);

	ConsumableType GetItemType() const { return TypeOfItem; }
	AmmoType GetAmmoType() const { return TypeOfAmmo; }
	int32 GetMaximumNum() const { return MaxAmount; }
	float GetWeightOfItem() const { return UnitWeight * CurrentAmount; }

	int32 GetCurrentNum() const { return CurrentAmount; }
	void SetCurrentNum(const int32 NewNum);
	UStaticMeshComponent* GetOutlineMesh() { return StaticMeshOutline; }

	FOnChangedItemDelegate OnChangedItem;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Comsumable | Display")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Comsumable | Display")
	UStaticMeshComponent* StaticMeshOutline;

	UPROPERTY(EditAnywhere, Category = "Consumable | Stat")
	ConsumableType TypeOfItem;

	UPROPERTY(EditAnywhere, Category = "Consumable | Stat")
	AmmoType TypeOfAmmo;

	UPROPERTY(EditAnywhere, Category = "Consumable | Stat")
	int32 MaxAmount;

	UPROPERTY(EditAnywhere, Category = "Consumable | Stat")
	int32 CurrentAmount;

	UPROPERTY(EditAnywhere, Category = "Consumable | Stat")
	float UnitWeight;
};
