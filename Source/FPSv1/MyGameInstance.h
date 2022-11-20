// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */

USTRUCT()
struct FMyCharacterData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FName Name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Attack;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MaxHp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 CurrentHp;
};

UCLASS()
class FPSV1_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void StartGameInstance() override;

	FMyCharacterData* GetStatData(FName Name);

private:
	UDataTable* MyStat;

};
