// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "MyCharacter.h"
#include "InventoryItemWidget.h"
#include "InventoryItemObject.h"
#include "ItemWidgetDrag.h"
#include "DraggedItemWidget.h"
#include "Inventory.h"
#include "Weapon.h"
#include "Item.h"
#include "Consumable.h"

#include "Engine/World.h"
#include "ItemListView.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"


void UInventoryWidget::BindPlayerCharacter(AMyCharacter* character)
{
	TargetCharacter = character;
}

void UInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	InventoryListView->ClearListItems();

	TargetInventory = TargetCharacter->GetInventory()->GetInventoryArray();
	for (AItem* item : TargetInventory)
	{
		AddItemToInventoryListView(item);
	}

	NearbyItemListView->ClearListItems();

	TArray<FHitResult> Out;
	FVector Start(TargetCharacter->GetActorLocation());
	Start.Z = 130.f;
	FVector End(Start + FVector(0.f, 0.f, 100.f));
	UKismetSystemLibrary::CapsuleTraceMultiByProfile(GetWorld(), Start, Start, 200.f, 200.f, FName("Item"),
		false, TArray<AActor*>(), EDrawDebugTrace::None, Out, true);

	for (FHitResult hit : Out)
	{
		if (hit.Actor.Get()->IsA<AItem>() == true)
			AddItemToNearbyListView(Cast<AItem>(hit.Actor.Get()));
	}

	//TArray<FOverlapResult> OverlapOut;
	//GetWorld()->OverlapMultiByChannel(OverlapOut, Start, FQuat::Identity,
	//	ECollisionChannel::ECC_GameTraceChannel3, FCollisionShape::MakeCapsule(FVector(200.f, 200.f, 200.f)));

	//for (FOverlapResult overlap : OverlapOut)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *overlap.Actor->GetName());
	//}


	RefreshWeaponSlot(WeaponSlotOne, FIRST_SLOT);
	RefreshWeaponSlot(WeaponSlotTwo, SECOND_SLOT);
	RefreshWeaponSlot(WeaponSlotThree, THIRD_SLOT);
}

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UItemWidgetDrag* ItemDragDropOp = Cast<UItemWidgetDrag>(InOperation);
	UUserWidget* TargetItemWidget = ItemDragDropOp->ItemWidgetRef;

	FVector2D StartDragPos = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry, ItemDragDropOp->DragOffset);
	FVector2D OnDropPos = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry, InDragDropEvent.GetScreenSpacePosition());
	int32 FromWidgetIdx = GetWidgetFrom(StartDragPos);
	int32 ToWidgetIdx = GetWidgetFrom(OnDropPos);

	if (FromWidgetIdx == ToWidgetIdx)
	{
		return true;
	}

	if (FromWidgetIdx == Nearby)
	{
		UInventoryItemWidget* ThisItemWidget = Cast<UInventoryItemWidget>(TargetItemWidget);
		if (ToWidgetIdx == Inventory
			&& ThisItemWidget->ItemRef->IsA<AConsumable>()
			&& ThisItemWidget->ItemRef->GetClass()->ImplementsInterface(UInteract::StaticClass()))
		{
			ThisItemWidget->ItemRef->PickUp_Implementation();

			return true;
		}
		if ((ToWidgetIdx == WSlotOne || ToWidgetIdx == WSlotTwo || ToWidgetIdx == WSlotThree)
			&& ThisItemWidget->ItemRef->IsA<AWeapon>()
			&& ThisItemWidget->ItemRef->GetClass()->ImplementsInterface(UInteract::StaticClass()))
		{
			ThisItemWidget->ItemRef->PickUp_Implementation(ToWidgetIdx - WSlotOne);

			return true;
		}
	}
	else if (FromWidgetIdx == Inventory)
	{
		UInventoryItemWidget* ThisItemWidget = Cast<UInventoryItemWidget>(TargetItemWidget);
		if (ToWidgetIdx == Nearby
			&& ThisItemWidget->ItemRef->GetClass()->ImplementsInterface(UInteract::StaticClass()))
		{
			ThisItemWidget->ItemRef->Drop_Implementation();
			return true;
		}
	}
	else if (FromWidgetIdx == WSlotOne || FromWidgetIdx == WSlotTwo || FromWidgetIdx == WSlotThree)
	{
		UWeaponSlot* ThisItemWidget = Cast<UWeaponSlot>(TargetItemWidget);
		if (ToWidgetIdx == Nearby
			&& ThisItemWidget->ItemRef->GetClass()->ImplementsInterface(UInteract::StaticClass()))
		{
			ThisItemWidget->ItemRef->Drop_Implementation();
			return true;
		}

		if (((FromWidgetIdx == WSlotOne && ToWidgetIdx == WSlotTwo)
			|| (FromWidgetIdx == WSlotTwo && ToWidgetIdx == WSlotOne))
			&& ThisItemWidget->ItemRef->GetClass()->ImplementsInterface(UInteract::StaticClass()))
		{
			TargetCharacter->GetInventory()->ChageItemLocation();
		}
	}
	return false;
}

FReply UInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton || InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		return FReply::Handled();

	return FReply::Unhandled();
}

int32 UInventoryWidget::GetWidgetFrom(FVector2D DragOffset)
{
	const FGeometry NearbyListViewGeometry = NearbyItemListView->GetPaintSpaceGeometry();
	FVector2D NearbyLocalPos = NearbyListViewGeometry.GetLocalPositionAtCoordinates(FVector2D(0, 0));

	const FGeometry InventoryListViewGeometry = InventoryListView->GetPaintSpaceGeometry();
	FVector2D InventoryLocalPos = InventoryListViewGeometry.GetLocalPositionAtCoordinates(FVector2D(0, 0));

	const FGeometry WSlotOneGeometry = WeaponSlotOne->GetPaintSpaceGeometry();
	FVector2D WSlotOneLocalPos = WSlotOneGeometry.GetLocalPositionAtCoordinates(FVector2D(0, 0));

	const FGeometry WSlotTwoGeometry = WeaponSlotTwo->GetPaintSpaceGeometry();
	FVector2D WSlotTwoLocalPos = WSlotTwoGeometry.GetLocalPositionAtCoordinates(FVector2D(0, 0));

	const FGeometry WSlotThreeGeometry = WeaponSlotThree->GetPaintSpaceGeometry();
	FVector2D WSlotThreeLocalPos = WSlotThreeGeometry.GetLocalPositionAtCoordinates(FVector2D(0, 0));


	FVector2D OffsetFromWidget = DragOffset - NearbyLocalPos;
	FVector2D WidgetSize = NearbyListViewGeometry.GetLocalSize() * NearbyListViewGeometry.Scale;
	if ((0 < OffsetFromWidget.X && OffsetFromWidget.X < WidgetSize.X) && (0 < OffsetFromWidget.Y && OffsetFromWidget.Y < WidgetSize.Y))
		return Nearby;


	OffsetFromWidget = DragOffset - InventoryLocalPos;
	WidgetSize = InventoryListViewGeometry.GetLocalSize() * InventoryListViewGeometry.Scale;
	if ((0 < OffsetFromWidget.X && OffsetFromWidget.X < WidgetSize.X) && (0 < OffsetFromWidget.Y && OffsetFromWidget.Y < WidgetSize.Y))
		return Inventory;


	OffsetFromWidget = DragOffset - WSlotOneLocalPos;
	WidgetSize = WSlotOneGeometry.GetLocalSize() * WSlotOneGeometry.Scale;
	if ((0 < OffsetFromWidget.X && OffsetFromWidget.X < WidgetSize.X) && (0 < OffsetFromWidget.Y && OffsetFromWidget.Y < WidgetSize.Y))
		return WSlotOne;


	OffsetFromWidget = DragOffset - WSlotTwoLocalPos;
	WidgetSize = WSlotTwoGeometry.GetLocalSize() * WSlotTwoGeometry.Scale;
	if ((0 < OffsetFromWidget.X && OffsetFromWidget.X < WidgetSize.X) && (0 < OffsetFromWidget.Y && OffsetFromWidget.Y < WidgetSize.Y))
		return WSlotTwo;


	OffsetFromWidget = DragOffset - WSlotThreeLocalPos;
	WidgetSize = WSlotThreeGeometry.GetLocalSize() * WSlotThreeGeometry.Scale;
	if ((0 < OffsetFromWidget.X && OffsetFromWidget.X < WidgetSize.X) && (0 < OffsetFromWidget.Y && OffsetFromWidget.Y < WidgetSize.Y))
		return WSlotThree;


	return -1;
}

void UInventoryWidget::AddItemToInventoryListView(AItem* TargetItem)
{
	AConsumable* TargetConsumableItem = Cast<AConsumable>(TargetItem);
	UInventoryItemObject* ItemObject = NewObject<UInventoryItemObject>(this, UInventoryItemObject::StaticClass());
	ItemObject->ItemName = TargetConsumableItem->GetNameOfItem();
	ItemObject->ItemCount = TargetConsumableItem->GetCurrentNum();
	ItemObject->ItemImgPath = TargetConsumableItem->GetItemTexture();
	ItemObject->ItemRef = TargetItem;
	InventoryListView->AddItem(ItemObject);
}

void UInventoryWidget::AddItemToNearbyListView(AItem* TargetItem)
{
	UInventoryItemObject* ItemObject = NewObject<UInventoryItemObject>(this, UInventoryItemObject::StaticClass());
	ItemObject->ItemName = TargetItem->GetNameOfItem();
	ItemObject->ItemImgPath = TargetItem->GetImageTexture();
	ItemObject->ItemRef = TargetItem;
	if (TargetItem->IsA<AConsumable>() == true)
	{
		AConsumable* TargetConsumableItem = Cast<AConsumable>(TargetItem);
		ItemObject->ItemCount = TargetConsumableItem->GetCurrentNum();
	}
	NearbyItemListView->AddItem(ItemObject);
}

void UInventoryWidget::RefreshWeaponSlot(UWeaponSlot* WSlot, int SlotNum)
{
	AWeapon** WInventory = TargetCharacter->GetInventory()->GetWeaponInventory();
	AWeapon* WeaponOne = WInventory[SlotNum];
	if (WeaponOne == nullptr)
	{
		//WSlot->SetVisibility(ESlateVisibility::Hidden);
		WSlot->SlotNum->SetText(FText::AsNumber(SlotNum + 1));
		WSlot->WeaponName->SetText(FText::GetEmpty());
		WSlot->AmmoType->SetText(FText::GetEmpty());
		WSlot->CurrentAmmo->SetText(FText::GetEmpty());
		WSlot->MaximumAmmo->SetText(FText::GetEmpty());
		WSlot->WeaponImage->SetRenderOpacity(0.f);
		WSlot->WeaponImage->SetBrushTintColor(FSlateColor::UseForeground());
		WSlot->ItemRef = nullptr;

	}
	else
	{
		WSlot->SlotNum->SetText(FText::AsNumber(SlotNum + 1));
		WSlot->WeaponName->SetText(FText::FromString(WeaponOne->GetNameOfItem().ToString()));
		AmmoType TypeOfAmmo = WeaponOne->GetType();
		FText AmmoTypeText;
		switch (TypeOfAmmo)
		{
		case AmmoType::Ammo5_56:
			AmmoTypeText.FromString("5.56mm");
			break;
		case AmmoType::Ammo7_62:
			AmmoTypeText.FromString("7.62mm");
			break;
		case AmmoType::Ammo9:
			AmmoTypeText.FromString("9mm");
			break;
		default:
			check(false);
			break;
		}
		WSlot->AmmoType->SetText(AmmoTypeText);
		WSlot->CurrentAmmo->SetText(FText::AsNumber(WeaponOne->GetCurrentAmmo()));
		WSlot->MaximumAmmo->SetText(FText::AsNumber(WeaponOne->GetMaximumAmmo()));
		WSlot->WeaponImage->SetRenderOpacity(1.f);
		WSlot->WeaponImage->SetBrushFromTexture(WeaponOne->GetImageTexture());
		WSlot->ItemRef = WeaponOne;
	}
}
