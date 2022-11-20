// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	const ConstructorHelpers::FObjectFinder<UDataTable> ST(TEXT("DataTable'/Game/Data/StatData.StatData'"));
	if (ST.Succeeded())
	{
		MyStat = ST.Object;
	}
}

void UMyGameInstance::Init()
{
	Super::Init();
}

void UMyGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UMyGameInstance::StartGameInstance()
{
	Super::StartGameInstance();
}

FMyCharacterData* UMyGameInstance::GetStatData(FName Name)
{
	//UE_LOG(LogTemp, Warning, TEXT("UMyGameInstance::GetStatData() called"));
	TArray<FMyCharacterData*> MyCharacterDataArray;
	MyStat->GetAllRows<FMyCharacterData>(TEXT(""), MyCharacterDataArray);
	for (auto Data : MyCharacterDataArray)
	{
		UE_LOG(LogTemp, Warning, TEXT("Name = %s, Level = %d"), *Data->Name.ToString(), Data->Level);
	}
	return MyStat->FindRow<FMyCharacterData>(Name, TEXT(""));
}
