// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "MyConstant.h"
#include "Weapon.h"
#include "MyCharacter.generated.h"

UENUM(BlueprintType)
enum class PerspectiveMode : uint8
{
	FirstPerson = 0,
	ThridPerson = 1,
};

UENUM(BlueprintType)
enum class ShotMode : uint8
{
	Single = 0,
	Continuous = 1,
};

UENUM(BlueprintType)
enum class WeaponState : uint8
{
	Default = 0,
	FirstWeapon = 1,
	SecondaryWeapon = 2,
};

DECLARE_DELEGATE_OneParam(FInputSwitchWeaponDelegate, const int32);
DECLARE_DELEGATE_OneParam(FDeleOneParam, const FText&);
DECLARE_DELEGATE(FDeleNoParam);

UCLASS()
class FPSV1_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void TimelineProgress(float Value);

	UFUNCTION()
	void TimelineProgressVector(FVector Value);

	UFUNCTION()
	void SwitchWeapon(const int32 WeaponIndex);

	UFUNCTION()
	void GetWeapon(int32 WeaponSlotNum);

	UFUNCTION()
	void DropWeapon(int32 TargetWeaponSlotNum);

	// Call by AnimInstance Class
	float GetHorizontalValue() const { return LeftRightValue; }
	float GetVerticalValue() const { return UpDownValue; }
	
	float GetPitch() const { return AimPitch; }
	float GetYaw() const { return AimYaw; }

	void SetCanSwitchingPose(bool bCanSwitchingPose) { CanSwitchingPose = bCanSwitchingPose; }

	bool GetIsDying() const { return IsDying; }
	bool GetIsProning() const { return IsProning; }
	bool GetIsCrouching() const { return IsCrouching; }

	bool GetIsTurningRight() const { return IsTurningRight; }
	bool GetIsTurningLeft() const { return IsTurningLeft; }

	int32 GetCurrentWeaponSlot() const { return CurrentWeaponSlot; }
	FVector GetLeftHandTarget() const { return WeaponLeftHandTarget; }

	void DamagedFromPlayer(FName HitPlayerName, float HitDamage);
	float GetPlayerDamage() const { return CurrentWeaponCanNull ? CurrentWeaponCanNull->GetDamage() : 0.5f; }

	class UAnimMontage* GetCurrentAttachMontage(int WeaponIdx) const;

	class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	class UCameraComponent* GetTPCamera() const { return TPCamera; }
	class UCameraComponent* GetFPCamera() const { return FPCamera; }
	class UInventory* GetInventory() const { return PlayerInventory; }
	class UMyStatComponent* GetStatComponent() const { return StatComponent; }
	class UMyCharacterStatWidget* GetStatWidget() const { return StatWidget; }

protected:
	virtual void BeginPlay() override;

private:
	// Binded Action with user input
	void Death();
	void Fire();
	void Sprint();
	void UnSprint();
	void MyJump();
	void MyCrouch();
	void MyProne();
	void Scope();
	void ShoulderHoldOn();
	void ShoulderHoldOff();
	void MyReload();
	void Interact();
	void OpenInventory();
	void ChangeShotMode();

	// Binded Axis with user input 
	void UpDown(float Value);
	void LeftRight(float Value);
	void AddYawInput(float Value);
	void AddPitchInput(float Value);

	void SyncControlActorOrient();
	void SetPerspectiveMode();

	void UpdatePitchLimits(float NewMinPitch, float NewMaxPitch);


public:
	FDeleNoParam FuncDeleSingleWeapon;
	FDeleNoParam FuncDeleSingleNoWeapon;
	FDeleOneParam FuncDeleNotifyOneParam;

private:
	// Camera components for TPS + FPS
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FPCamera;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* TPCamera;


	// Pre Loaded Montages for Action Animation
	UPROPERTY(VisibleAnywhere, Category = Action)
	class UAnimMontage* PunchingMontage;

	UPROPERTY(VisibleAnywhere, Category = Action)
	class UAnimMontage* FiringMontage;

	UPROPERTY(VisibleAnywhere, Category = Action)
	class UAnimMontage* SecondaryFiringMontage;

	UPROPERTY(VisibleAnywhere, Category = Action)
	class UAnimMontage* CurrentAttackMontage;

	UPROPERTY(VisibleAnywhere, Category = Action)
	class UAnimMontage* ReloadingMontage;


	// Inventory Component and Widget
	UPROPERTY(VisibleAnywhere)
	class UInventory* PlayerInventory;
	UPROPERTY()
	TSubclassOf<class UUserWidget> InventoryWidgetClass;
	UPROPERTY()
	class UInventoryWidget* InventoryWidget;
	UPROPERTY(EditAnywhere)
	class USphereComponent* ScanItemComponent;


	// Player Weapon which can be null 
	UPROPERTY()
	class AWeapon* CurrentWeaponCanNull;

	// Components and Widget about Player Stat
	UPROPERTY(VisibleAnyWhere)
	class UMyStatComponent* StatComponent;
	UPROPERTY()
	class TSubclassOf<class UUserWidget> StatWidgetClass;
	UPROPERTY(VisibleAnyWhere)
	class UMyCharacterStatWidget* StatWidget;
	UPROPERTY()
	class TSubclassOf<class UUserWidget> NotiWidgetClass;

	UPROPERTY()
	TSubclassOf<AActor> InteractTestActorClass;

private:
	// for animation
	FRotator BaseActorRotation;

	float AimPitch;
	float AimYaw;
	float UpDownValue;
	float LeftRightValue;
	bool CanSwitchingPose = true;
	
	bool IsTurningRight;
	bool IsTurningLeft;
	bool IsScopeMode = false;

	UPROPERTY(EditAnywhere, Category = "BasePose")
	bool IsDying = false;
	UPROPERTY(EditAnywhere, Category = "BasePose")
	bool IsCrouching = false;
	UPROPERTY(EditAnywhere, Category = "BasePose")
	bool IsProning = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	int32 CurrentWeaponSlot = NONE_WEAPON;

	FVector WeaponLeftHandTarget;

	PerspectiveMode CurrentPerspectiveMode;

	ShotMode CurrentShotMode = ShotMode::Single;

	// for Moving by timeline (interpolation)
	FTimeline CurveTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* CurveFloat_CameraArmLength;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveVector* CurveVector_CameraSocketOffset;
	UPROPERTY()
	FVector StartLoc;
	UPROPERTY()
	FVector EndLoc;

	// for loading stat component
	UPROPERTY(EditAnyWhere)
	FName PlayerName;

	UPROPERTY()
	FTimerHandle myTimerHandle;
};
