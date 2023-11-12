// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Containers/Array.h"
#include "WeaponSlot.h"
#include "InventoryWidget.generated.h"

UCLASS()
class FPSV1_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindPlayerCharacter(class AMyCharacter* character);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	int32 GetWidgetFrom(FVector2D DragOffset);

	void AddItemToInventoryListView(class AItem* TargetItem);
	void AddItemToNearbyListView(class AItem* TargetItem);
	void RefreshWeaponSlot(UWeaponSlot* WSlot, int SlotNum);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UItemListView* NearbyItemListView;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UItemListView* InventoryListView;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UWeaponSlot* WeaponSlotOne;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UWeaponSlot* WeaponSlotTwo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UWeaponSlot* WeaponSlotThree;

private:
	UPROPERTY()
	TWeakObjectPtr<class AMyCharacter> TargetCharacter;
	UPROPERTY()
	TArray<class AItem*> TargetInventory;

	enum {	Nearby = 0,
			Inventory = 1,
			WSlotOne = 2,
			WSlotTwo = 3,
			WSlotThree = 4 };
};
