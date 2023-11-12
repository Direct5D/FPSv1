// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSv1DebugViewWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSV1_API UFPSv1DebugViewWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void RefreshListView();
	void AddToListCharacterData(FString _TargetName, FString _TargetContent);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UListView* DebugListView;
	//class AFPSv1Character* TargetPlayer;
	class AMyCharacter* TargetPlayer;
	class UCameraComponent* TargetFPCamera;
	class USpringArmComponent* TargetCameraBoom;

private:
	//FRotator ControlRotation;
};
