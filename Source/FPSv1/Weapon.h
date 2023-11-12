// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"


//Base Weapon Actor class which is Abstract
UCLASS()
class FPSV1_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:	
	AWeapon();

	void LoadWeaponData(FName ConsumableName, FString WeaponMeshPath, WeaponType WeaponType, float WeaponDamage, AmmoType WeaponAmmoType, int32 MaximumAmmoNum, FString TexturePath);

	USkeletalMeshComponent* GetSkeletalMeshComponent() const { return SkeletalMesh; }

	float GetDamage() const { return Damage; }
	void SetDamage(const float NewDamage) { Damage = NewDamage; }

	AmmoType GetType() const { return TypeOfAmmo; }
	void SetAmmoType(const AmmoType NewAmmoType) { TypeOfAmmo = NewAmmoType; }

	int32 GetMaximumAmmo() const { return MaxNumAmmo; }
	void SetMaximumAmmo(const int32 NewMaximumAmmo) { MaxNumAmmo = NewMaximumAmmo; }

	int32 GetCurrentAmmo() const { return CurrentRemainingAmmo; }
	void SetCurrentRemainingAmmo(const int32 NewRemainingAmmo) { CurrentRemainingAmmo = NewRemainingAmmo; }

	WeaponType GetTypeOfWeapon() const { return TypeOfWeapon; }
	void SetTypeOfWeapon(const WeaponType NewWeaponType) { TypeOfWeapon = NewWeaponType; }

	int32 GetDeltaAmmo() const { return MaxNumAmmo - CurrentRemainingAmmo; }
	void ReloadAmmo(const int32 Num) { CurrentRemainingAmmo += Num; }
	void SetConsumeAAmmo() { CurrentRemainingAmmo -= 1; }

	USkeletalMeshComponent* GetOutlineMesh() { return SkeletalMeshOutline; }
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Weapon | Display")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon | Display")
	USkeletalMeshComponent* SkeletalMeshOutline;

	UPROPERTY(EditAnywhere, Category = "Weapon | Stat")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Weapon | Stat")
	AmmoType TypeOfAmmo;

	UPROPERTY(EditAnywhere, Category = "Weapon | Stat")
	int32 MaxNumAmmo;

	UPROPERTY(EditAnywhere, Category = "Weapon | Stat")
	int32 CurrentRemainingAmmo;

	UPROPERTY(EditAnywhere, Category = "Weapon | Stat")
	WeaponType TypeOfWeapon;
};
