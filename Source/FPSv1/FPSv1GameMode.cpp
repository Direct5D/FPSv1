// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSv1GameMode.h"
#include "FPSv1GameLog.h"
#include "FPSv1PlayerController.h"
#include "FPSv1Character.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"


AFPSv1GameMode::AFPSv1GameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_MyCharacter"));
	//static ConstructorHelpers::FClassFinder<AHUD> HUDBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_FPSv1HUD"));
	
	//if (PlayerPawnBPClass.Class && HUDBPClass.Class)
	if (PlayerPawnBPClass.Class)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		//HUDClass = HUDBPClass.Class;
	}
	PlayerControllerClass = AFPSv1PlayerController::StaticClass();
}

FVector GenerateRandomLocation()
{
	float X = FMath::RandRange(-1000.f, 1000.f);
	float Y = FMath::RandRange(-1000.f, 1000.f);

	return FVector(X, Y, 300.f);
}

void AFPSv1GameMode::BeginPlay()
{
	//GEngine->GetGameUserSettings();
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	FVector RandomLocation = GenerateRandomLocation();
	FVector RandomLocation2 = GenerateRandomLocation();
	FVector RandomLocation3 = GenerateRandomLocation();
	FRotator SpawnRotation(-90.0f, 0.0f, 0.0f);

	UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());
	for (FWeaponData* TargetWeapon : MyGameInstance->GetWeaponDataArray())
	{
		for (int i = 0; i < 2; i++)
		{
			AWeapon* SpawnWeapon = GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass(), GenerateRandomLocation(), SpawnRotation);

			SpawnWeapon->LoadWeaponData(TargetWeapon->WeaponName, TargetWeapon->WeaponMeshPath,
										TargetWeapon->WeaponType, TargetWeapon->WeaponDamage, 
										TargetWeapon->WeaponAmmoType, TargetWeapon->MaximumAmmoNum, TargetWeapon->TexturePath);
		}
	}

	FRotator SpawnRotation2(0.0f, 0.0f, 0.0f);
	for (FConsumableData* TargetConsumable : MyGameInstance->GetConsumableDataArray())
	{
		for (int i = 0; i < 3; i++)
		{
			AConsumable* SpawnConsumable = GetWorld()->SpawnActor<AConsumable>(AConsumable::StaticClass(), GenerateRandomLocation(), SpawnRotation2);

			SpawnConsumable->LoadConsumableData(TargetConsumable->ConsumableName, TargetConsumable->ConsumableMeshPath,
												TargetConsumable->ConsumableType, TargetConsumable->TypeOfAmmo, TargetConsumable->MaximumNum,
												TargetConsumable->CurrentNum, TargetConsumable->WeightOfItem, TargetConsumable->TexturePath);
		}
	}
}
