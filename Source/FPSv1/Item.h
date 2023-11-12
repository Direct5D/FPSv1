// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interact.h"
#include "Components/ChildActorComponent.h"
#include "Engine/Texture2D.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"


UENUM(Blueprintable)
enum class WeaponType : uint8
{
	MainWeapon = 0,
	SecondaryWeapon = 1,
	Melee = 2,
};

UENUM(Blueprintable)
enum class AmmoType : uint8
{
	Ammo5_56 = 0,
	Ammo7_62 = 1,
	Ammo9 = 2,
	None = 3
};

UCLASS(Abstract)
class FPSV1_API AItem : public AActor, public IInteract
{
	GENERATED_BODY()
	
public:
	FName GetNameOfItem() const { return NameOfItem; }
	UTexture2D* GetItemTexture() const { return ImageTexture; }

	virtual bool PickUp_Implementation(int32 weaponIdx = -1) override;
	virtual bool Drop_Implementation() override;
	virtual bool Detect_Implemenetation() override;
	
	//UFUNCTION()
	//virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//UFUNCTION()
	//virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FName GetItemName() const { return NameOfItem; }
	UTexture2D* GetImageTexture() const { return ImageTexture; }

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Basic Item Property")
	FName NameOfItem;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Basic Item Property")
	UTexture2D* ImageTexture;

//	UPROPERTY(EditAnywhere)
//	UChildActorComponent* PopupWidget;
};
