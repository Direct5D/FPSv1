// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStatComponent.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

UMyStatComponent::UMyStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UMyStatComponent::SetStat(const FName _Name)
{
	if (_Name.IsNone() != true)
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (MyGameInstance)
		{
			FMyCharacterData* MyCharacterData = MyGameInstance->GetStatDataByName(_Name);
			if (MyCharacterData)
			{
				Name = MyCharacterData->Name;
				MaxHp = MyCharacterData->MaxHp;
				CurrentHp = MyCharacterData->CurrentHp;
				CurrentAmmo = 0;
				MaximumAmmo = 0;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("_Name is NAME_NONE"));
	}
}

void UMyStatComponent::SetNewHp(const float NewHp)
{
	CurrentHp = NewHp < KINDA_SMALL_NUMBER ? 0 : NewHp;
	OnHPChanged.Broadcast();
	if (CurrentHp == 0)
	{
		OnDeath.Broadcast();
	}
}

void UMyStatComponent::SetCurrentAmmo(const int32 NewAmmo)
{
	CurrentAmmo = NewAmmo;

	OnAmmoChanged.Broadcast();
}

void UMyStatComponent::SetNewWeapon(const int32 NewMaximumAmmo, const int32 NewAmmo)
{
	MaximumAmmo = NewMaximumAmmo;
	CurrentAmmo = NewAmmo;

	OnAmmoChanged.Broadcast();
}
