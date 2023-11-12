// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "DraggedItemWidget.h"
#include "ItemWidgetDrag.h"
#include "Item.h"

void UWeaponSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UDraggedItemWidget* DraggedWidget = Cast<UDraggedItemWidget>(CreateWidget(this, DraggedWidgetClass));
	if (DraggedWidget)
	{
		DraggedWidget->ItemWidgetRef = this;
		DraggedWidget->DraggedItemImage->SetBrush(WeaponImage->Brush);
		DraggedWidget->DraggedItemNameTxt->SetText(WeaponName->Text);
		//DraggedWidget->DraggedItemCountTxt->SetText(ItemCountTxt->Text);

		//UItemWidgetDrag* ItemDragDropOp = Cast<UItemWidgetDrag>(UWidgetBlueprintLibrary::CreateDragDropOperation(UItemWidgetDrag::StaticClass()));
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

FReply UWeaponSlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && ItemRef != nullptr)
	{
		// Detect if a drag operation has started.
		FReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;

		if (Reply.IsEventHandled())
		{
			DragOffset = InMouseEvent.GetScreenSpacePosition();
			return Reply;
		}
	}

	return FReply::Unhandled();
}