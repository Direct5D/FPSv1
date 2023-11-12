// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "InventoryItemWidget.generated.h"

UCLASS()
class FPSV1_API UInventoryItemWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ItemImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ItemNameTxt;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ItemCountTxt;
	UPROPERTY(BlueprintReadWrite)
	class AItem* ItemRef;
	UPROPERTY(BlueprintReadWrite)
	FVector2D DragOffset;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UUserWidget> DraggedWidgetClass;
};
