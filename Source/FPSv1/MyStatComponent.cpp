// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStatComponent.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMyStatComponent::UMyStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	UE_LOG(LogTemp, Warning, TEXT("UMyStatComponent::UMyStatComponent() called"));
	// ...
}


void UMyStatComponent::SetStat(FName _Name)
{
	if (_Name.IsNone() != true)
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (nullptr != MyGameInstance)
		{
			FMyCharacterData* MyCharacterData = MyGameInstance->GetStatData(_Name);
			if (MyCharacterData)
			{
				Name = MyCharacterData->Name;
				Level = MyCharacterData->Level;
				Attack = MyCharacterData->Attack;
				MaxHp = MyCharacterData->MaxHp;
				CurrentHp = MyCharacterData->CurrentHp;

				UE_LOG(LogTemp, Warning,
					TEXT("[UMyStatComponent::SetStat Name=[%s], Level=[%d], Attack=[%d], MaxHp=[%d], CurrentHp=[%d]"),
					*_Name.ToString(), Level, Attack, MaxHp, CurrentHp);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("_Name is NAME_NONE"));
	}
}

void UMyStatComponent::SetNewHp(float NewHp)
{
	CurrentHp = NewHp > 0 ? NewHp : 0;
}

float UMyStatComponent::GetHpRatio()
{
	return CurrentHp / (float)MaxHp;
}

FName UMyStatComponent::GetName()
{
	return Name;
}

int32 UMyStatComponent::GetLevel()
{
	return Level;
}

int32 UMyStatComponent::GetAttack()
{
	return Attack;
}

int32 UMyStatComponent::GetMaxHp()
{
	return MaxHp;
}

int32 UMyStatComponent::GetCurrentHp()
{
	return CurrentHp;
}

// Called when the game starts
void UMyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMyStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	UE_LOG(LogTemp, Warning, TEXT("UMyStatComponent::InitializeComponent() called"));
}


// Called every frame
void UMyStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

