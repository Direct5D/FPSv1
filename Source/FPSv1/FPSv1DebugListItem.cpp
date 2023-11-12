// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSv1DebugListItem.h"
#include "FPSv1DebugListObject.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/UserWidget.h"


void UFPSv1DebugListItem::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFPSv1DebugListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (m_pDebugTxt && m_pDebugContents)
	{
		UFPSv1DebugListObject* pListItemObject = Cast<UFPSv1DebugListObject>(ListItemObject);

		m_pDebugTxt->SetText(FText::FromString(pListItemObject->TargetName));
		m_pDebugContents->SetText(FText::FromString(pListItemObject->TargetContents));
	}
}
