// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemWidget.h"
#include "InventoryItemObject.h"
#include "Engine/World.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "DraggedItemWidget.h"
#include "ItemWidgetDrag.h"
#include "Item.h"


void UInventoryItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (ItemImage && ItemNameTxt && ItemCountTxt)
	{
		UInventoryItemObject* ItemObject = Cast<UInventoryItemObject>(ListItemObject);
		ItemImage->SetBrushFromTexture(ItemObject->ItemImgPath);
		ItemNameTxt->SetText(FText::FromName(ItemObject->ItemName));
		if (ItemObject->ItemCount == 0)
			ItemCountTxt->SetText(FText::GetEmpty());
		else
			ItemCountTxt->SetText(FText::FromString(FString::FromInt(ItemObject->ItemCount)));
		ItemRef = ItemObject->ItemRef;
	}
}

void UInventoryItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UDraggedItemWidget* DraggedWidget = Cast<UDraggedItemWidget>(CreateWidget(this, DraggedWidgetClass));
	if (DraggedWidget)
	{
		DraggedWidget->ItemWidgetRef = this;
		DraggedWidget->DraggedItemImage->SetBrush(ItemImage->Brush);
		DraggedWidget->DraggedItemNameTxt->SetText(ItemNameTxt->Text);
		DraggedWidget->DraggedItemCountTxt->SetText(ItemCountTxt->Text);

		UItemWidgetDrag* ItemDragDropOp = NewObject<UItemWidgetDrag>();
		if (ItemDragDropOp)
		{
			ItemDragDropOp->DefaultDragVisual = DraggedWidget;
			ItemDragDropOp->ItemWidgetRef = this;
			ItemDragDropOp->DragOffset = DragOffset;
			ItemDragDropOp->Pivot = EDragPivot::CenterCenter;

			OutOperation = ItemDragDropOp;
		}
	}
}

FReply UInventoryItemWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		// Detect if a drag operation has started.
		FReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
		FVector2D LocalWidgetMousePos = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry, InMouseEvent.GetScreenSpacePosition());
		if (Reply.IsEventHandled())
		{
			
			DragOffset = InMouseEvent.GetScreenSpacePosition();
			//DragOffset = LocalWidgetMousePos;
			return Reply;
		}
	}

	return FReply::Unhandled();
}

FReply UInventoryItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{

		return FReply::Handled();
	}
	return FReply::Unhandled();
}