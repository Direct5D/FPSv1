// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProjectile.h"
#include "FPSv1Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyProjectile::AMyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(3.0f);
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
			StaticMeshComp->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
		}
	}


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	InitialLifeSpan = 3.0f;
}

void AMyProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		AActor* PlayerActor = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		//UE_LOG(LogTemp, Warning, TEXT("OtherActor Name : %s"), *OtherActor->GetName());
		//if(OtherActor == PlayerActor)
		if (OtherComp->ComponentHasTag(TEXT("Player")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Player has Shot"));
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Comp has Shot"));
			if (OtherComp->IsSimulatingPhysics())
			{
				//UE_LOG(LogTemp, Warning, TEXT("Simulating Comp has Shot"));
				OtherComp->AddImpulseAtLocation(GetVelocity() * 100.f, GetActorLocation());
			}

			Destroy();
		}
	}
}
