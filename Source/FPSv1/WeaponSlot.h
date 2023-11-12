// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "WeaponSlot.generated.h"

UCLASS()
class FPSV1_API UWeaponSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* SlotNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* WeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* AmmoType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CurrentAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MaximumAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* WeaponImage;
	UPROPERTY(BlueprintReadWrite)
	class AItem* ItemRef;
	UPROPERTY(BlueprintReadWrite)
	FVector2D DragOffset;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UUserWidget> DraggedWidgetClass;
};
