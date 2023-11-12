// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "GameFramework/Character.h"
#include "CapturePlayer.generated.h"



UCLASS()
class FPSV1_API ACapturePlayer : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACapturePlayer();

	int32 GetCurrentWeaponSlot() const { return CurrentWeaponSlot; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere)
	class AMyCharacter* OriginalCharacter;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class USceneCaptureComponent2D* SceneCaptureComp2D;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* WeaponSkMeshComps[MAXIMUM_SLOT + 1];

	UPROPERTY(VisibleAnywhere)
	int32 CurrentWeaponSlot = NONE_WEAPON;
};
