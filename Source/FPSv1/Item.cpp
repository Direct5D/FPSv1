// Copyright Epic Games, Inc. All Rights Reserved.

#include "Item.h"
#include "MyCharacter.h"
#include "Inventory.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"


bool AItem::PickUp_Implementation(int32 weaponIdx)
{
	SetActorEnableCollision(false);

	AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter == nullptr)
	{
		return false;
	}

	UInventory* PlayerInventory = PlayerCharacter->GetInventory();
	if (PlayerInventory == nullptr)
	{
		return false;
	}

	if (PlayerInventory->AddToInventory(this, weaponIdx) == true)
	{
		if (this->IsA(AWeapon::StaticClass()) != true)
		{
			SetActorTickEnabled(false);
			SetActorHiddenInGame(true);
		}
		UBoxComponent* BoxComp = Cast<UBoxComponent>(GetRootComponent());
		BoxComp->SetSimulatePhysics(false);
		return true;
	}

	SetActorEnableCollision(true);
	return false;
}

bool AItem::Drop_Implementation()
{
	AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter == nullptr)
	{
		return false;
	}
	UInventory* PlayerInventory = PlayerCharacter->GetInventory();

	if (PlayerInventory == nullptr)
	{
		return false;
	}

	if (PlayerInventory->RemoveFromInventory(this) == true)
	{
		float NewX = FMath::RandRange(PlayerCharacter->GetActorLocation().X - 30.f, PlayerCharacter->GetActorLocation().X + 30.f);
		float NewY = FMath::RandRange(PlayerCharacter->GetActorLocation().Y - 30.f, PlayerCharacter->GetActorLocation().Y + 30.f);
		FVector NewLoc = FVector(NewX, NewY, PlayerCharacter->GetActorLocation().Z);

		UBoxComponent* BoxComp = Cast<UBoxComponent>(GetRootComponent());
		BoxComp->SetSimulatePhysics(true);
		SetActorLocation(NewLoc);
		SetActorEnableCollision(true);
		SetActorTickEnabled(true);
		SetActorHiddenInGame(false);

		return true;
	}
	return false;
}

bool AItem::Detect_Implemenetation()
{
	
	


	/*if (this->isa<aconsumable>())
	{
		aconsumable* item = cast<aconsumable>(this);
		item->getoutlinemesh()->setvisibility(true);
	}
	else if (this->isa<aweapon>())
	{
		aweapon* item = cast<aweapon>(this);
		item->getoutlinemesh()->setvisibility(true);
	}*/

	return false;
}
