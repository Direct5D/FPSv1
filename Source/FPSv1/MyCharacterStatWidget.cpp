// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterStatWidget.h"
#include "NotificationWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "MyStatComponent.h"
#include "MyCharacter.h"
#include "Components/ProgressBar.h"

void UMyCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CurrentCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	CurrentCharacter->FuncDeleSingleWeapon.BindUFunction(this, FName("ShowCrossHair"));
	CurrentCharacter->FuncDeleSingleNoWeapon.BindUFunction(this, FName("HideCrossHair"));
	CurrentCharacter->FuncDeleNotifyOneParam.BindUFunction(this, FName("CreateNotification"));
	HideCrossHair();
	UpdateHPBar();
	UpdateAmmo();
}

void UMyCharacterStatWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector CharacterVelocity = CurrentCharacter->GetVelocity();
	float SpreadValue = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 450.f), FVector2D(0.f, 40.f), CharacterVelocity.Size());
	CrossHairSpreadOut(SpreadValue);
}

void UMyCharacterStatWidget::BindWithStat(UMyStatComponent* BindStatComponent)
{
	if (TargetStat != nullptr) return;
	
	TargetStat = BindStatComponent;
	TargetStat->OnHPChanged.AddUObject(this, &UMyCharacterStatWidget::UpdateHPBar);
	TargetStat->OnAmmoChanged.AddUObject(this, &UMyCharacterStatWidget::UpdateAmmo);
}

void UMyCharacterStatWidget::UpdateHPBar() const
{
	if (TargetStat == nullptr) return;
	
	HPBar->SetPercent(TargetStat->GetHpRatio());
}

void UMyCharacterStatWidget::UpdateAmmo() const
{
	if (TargetStat == nullptr) return;
	

	if (TargetStat->GetMaximumAmmo() == 0)
	{
		CurrentAmmo->SetVisibility(ESlateVisibility::Hidden);
		MaximumAmmo->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		CurrentAmmo->SetText(FText::AsNumber(TargetStat->GetCurrentAmmo()));
		MaximumAmmo->SetText(FText::AsNumber(TargetStat->GetMaximumAmmo()));

		CurrentAmmo->SetVisibility(ESlateVisibility::Visible);
		MaximumAmmo->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMyCharacterStatWidget::ShowCrossHair() const
{
	CH_Right->SetVisibility(ESlateVisibility::Visible);
	CH_Left->SetVisibility(ESlateVisibility::Visible);
	CH_Top->SetVisibility(ESlateVisibility::Visible);
	CH_Bottom->SetVisibility(ESlateVisibility::Visible);
}

void UMyCharacterStatWidget::HideCrossHair() const
{
	CH_Right->SetVisibility(ESlateVisibility::Hidden);
	CH_Left->SetVisibility(ESlateVisibility::Hidden);
	CH_Top->SetVisibility(ESlateVisibility::Hidden);
	CH_Bottom->SetVisibility(ESlateVisibility::Hidden);
}

void UMyCharacterStatWidget::CrossHairSpreadOut(float value) const
{
	CH_Right->SetRenderTranslation(FVector2D(value, 0.f));
	CH_Left->SetRenderTranslation(FVector2D(-value, 0.f));
	CH_Top->SetRenderTranslation(FVector2D(0.f, -value));
	CH_Bottom->SetRenderTranslation(FVector2D(0.f, value));
}

void UMyCharacterStatWidget::CreateNotification(const FText& message)
{
	UNotificationWidget* NotiChildWidget = Cast<UNotificationWidget>(CreateWidget(this, NotifyWidgetClass));
	if (NotiChildWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Success on create noti widget"));
		NotiChildWidget->SetMessage(message);
		NotificationPanel->AddChild(NotiChildWidget);
	}
}
