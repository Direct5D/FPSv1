// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProjectile.generated.h"


UCLASS()
class FPSV1_API AMyProjectile : public AActor
{
	GENERATED_BODY()

public:
	AMyProjectile();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, Category = Projectile)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
	class USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;
};
