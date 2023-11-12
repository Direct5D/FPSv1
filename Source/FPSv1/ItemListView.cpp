// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemListView.h"
#include "Engine/World.h"

void UItemListView::OnItemClickedInternal(UObject* Item)
{
	Super::OnItemClickedInternal(Item);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString("Inventory Item clicked"));
	}
}

/*
void UItemListView::HandleListEntryHovered(UUserWidget& EntryWidget)
{

}

void UItemListView::HandleListEntryUnhovered(UUserWidget& EntryWidget)
{

}
*/
