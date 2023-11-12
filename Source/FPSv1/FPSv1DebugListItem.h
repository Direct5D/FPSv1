// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "FPSv1DebugListItem.generated.h"

/**
 * 
 */
UCLASS()
class FPSV1_API UFPSv1DebugListItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* m_pDebugTxt;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* m_pDebugContents;
};
