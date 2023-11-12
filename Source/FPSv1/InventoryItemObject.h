// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Item.h"
#include "InventoryItemObject.generated.h"

UCLASS()
class FPSV1_API UInventoryItemObject : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemImgPath;
	UPROPERTY()
	AItem* ItemRef;
};
