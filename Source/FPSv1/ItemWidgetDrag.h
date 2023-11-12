// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryItemWidget.h"
#include "ItemWidgetDrag.generated.h"

/**
 * 
 */
UCLASS()
class FPSV1_API UItemWidgetDrag : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (InstanceEditable = "true", ExposeOnSpawn = "true"))
	FVector2D DragOffset;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (InstanceEditable = "true", ExposeOnSpawn = "true"))
	UUserWidget* ItemWidgetRef;

	enum { Nearby = 1, Inventory = 2 };
	int32 DraggedFrom;
};
