// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProjectile.h"
#include "MyCharacter.h"
#include "MyStatComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AMyProjectile::AMyProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(0.556f);
	CollisionComp->SetCollisionObjectType(ECC_GameTraceChannel1);
	CollisionComp->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComp->OnComponentHit.AddDynamic(this, &AMyProjectile::OnHit);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	if (StaticMeshComp)
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/ThirdPerson/Meshes/ProjectileMesh.FirstPersonProjectileMesh'"));
		if (SM.Succeeded())
		{	
			StaticMeshComp->SetStaticMesh(SM.Object);
			StaticMeshComp->SetupAttachment(RootComponent);
			StaticMeshComp->SetRelativeScale3D(FVector(0.005f, 0.005f, 0.005f));
		}
	}

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 50000.f;
	ProjectileMovement->MaxSpeed = 50000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	InitialLifeSpan = 3.0f;
}

void AMyProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		AMyCharacter* PlayerActor = Cast<AMyCharacter>(OtherActor);
		AMyCharacter* OwnerActor = Cast<AMyCharacter>(GetOwner());
		if (PlayerActor && OwnerActor)
		{
			
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Hit.BoneName.ToString());
			PlayerActor->DamagedFromPlayer(OwnerActor->GetStatComponent()->GetName(), OwnerActor->GetPlayerDamage());
		}
		
		if (OtherComp->IsSimulatingPhysics())
		{
			UE_LOG(LogTemp, Warning, TEXT("Simulating Comp has Shot"));
			OtherComp->AddImpulseAtLocation(GetVelocity() * 1.f, GetActorLocation());
		}
		DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 5.f, FColor::Red, false, 10.f);
	}
	Destroy();
}

void AMyProjectile::Tick(float DeltaSeconds)
{
	DrawDebugPoint(GetWorld(), GetActorLocation(), 5.f, FColor::Green, false, 10.f);
}
