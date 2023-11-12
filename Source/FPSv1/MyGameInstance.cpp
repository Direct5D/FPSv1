// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ST(TEXT("DataTable'/Game/Data/StatDataTable.StatDataTable'"));
	if (ST.Succeeded())
	{
		MyStat = ST.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponListDataTable(TEXT("DataTable'/Game/Data/WeaponDataTable.WeaponDataTable'"));
	if (WeaponListDataTable.Succeeded())
	{
		WeaponList = WeaponListDataTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> ConsumableListDataTable(TEXT("DataTable'/Game/Data/ConsumableDataTable.ConsumableDataTable'"));
	if (ConsumableListDataTable.Succeeded())
	{
		ConsumableList = ConsumableListDataTable.Object;
	}
}

FMyCharacterData* UMyGameInstance::GetStatDataByName(FName Name) const
{
	TArray<FMyCharacterData*> MyCharacterDataArray;
	MyStat->GetAllRows<FMyCharacterData>(TEXT(""), MyCharacterDataArray);
	for (auto Data : MyCharacterDataArray)
	{
		UE_LOG(LogTemp, Warning, TEXT("Name = %s"), *Data->Name.ToString());
	}
	return MyStat->FindRow<FMyCharacterData>(Name, TEXT(""));
}

TArray<FWeaponData*> UMyGameInstance::GetWeaponDataArray() const
{
	TArray<FWeaponData*> WeponDataArray;
	WeaponList->GetAllRows(TEXT(""), WeponDataArray);
	return WeponDataArray;
}

TArray<FConsumableData*> UMyGameInstance::GetConsumableDataArray() const
{
	TArray<FConsumableData*> ConsumableDataArray;
	ConsumableList->GetAllRows(TEXT(""), ConsumableDataArray);

	return ConsumableDataArray;
}
