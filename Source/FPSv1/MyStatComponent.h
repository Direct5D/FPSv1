// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSV1_API UMyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyStatComponent();
	void SetStat(FName Name);
	void SetNewHp(float NewHp);
	float GetHpRatio();
	FName GetName();
	int32 GetLevel();
	int32 GetAttack();
	int32 GetMaxHp();
	int32 GetCurrentHp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnyWhere, Category = Stat, Meta=(AllowPrivateAccess))
	FName Name;
	UPROPERTY(EditAnyWhere, Category = Stat, Meta = (AllowPrivateAccess))
	int32 Level;
	UPROPERTY(EditAnyWhere, Category = Stat, Meta = (AllowPrivateAccess))
	int32 Attack;
	UPROPERTY(EditAnyWhere, Category = Stat, Meta = (AllowPrivateAccess))
	int32 MaxHp;
	UPROPERTY(EditAnyWhere, Category = Stat, Meta = (AllowPrivateAccess))
	int32 CurrentHp;
};
