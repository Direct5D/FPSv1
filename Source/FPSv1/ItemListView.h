// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ListView.h"
#include "ItemListView.generated.h"


UCLASS()
class FPSV1_API UItemListView : public UListView
{
	GENERATED_BODY()
	
public:
	virtual void OnItemClickedInternal(UObject* Item) override;
//	virtual void HandleListEntryHovered(UUserWidget& EntryWidget) override;
//	virtual void HandleListEntryUnhovered(UUserWidget& EntryWidget) override;
};
