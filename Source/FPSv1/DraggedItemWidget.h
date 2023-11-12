// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemWidget.h"
#include "DraggedItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSV1_API UDraggedItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* DraggedItemImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DraggedItemNameTxt;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DraggedItemCountTxt;
	UPROPERTY(BlueprintReadWrite)
	class UUserWidget* ItemWidgetRef;
};
