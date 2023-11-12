// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyCharacterStatWidget.generated.h"


UCLASS()
class FPSV1_API UMyCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void BindWithStat(class UMyStatComponent* BindStatComponent);
	void UpdateHPBar() const;
	void UpdateAmmo() const;
	UFUNCTION()
	void ShowCrossHair() const;
	UFUNCTION()
	void HideCrossHair() const;
	
	void CrossHairSpreadOut(float value) const;

	UFUNCTION()
	void CreateNotification(const FText& message);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HPBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CurrentAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MaximumAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* CH_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* CH_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* CH_Top;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* CH_Bottom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* NotificationPanel;
	UPROPERTY()
	TSubclassOf<UUserWidget> NotifyWidgetClass;

private:
	class UMyStatComponent* TargetStat;
	class AMyCharacter* CurrentCharacter;
	FTimerHandle WidgetTimer;
};
