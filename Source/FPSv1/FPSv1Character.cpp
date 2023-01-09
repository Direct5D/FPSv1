// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSv1Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimInstance.h"
#include "MyProjectile.h"
#include "MyStatComponent.h"
#include "DrawDebugHelpers.h"


//////////////////////////////////////////////////////////////////////////
// AFPSv1Character

AFPSv1Character::AFPSv1Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;


	// Mesh, Gun
	CharacterMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (CharacterMesh.Succeeded())
	{
		CharacterMeshComponent->SetSkeletalMesh(CharacterMesh.Object);
	}
	
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FP_Gun_Mesh(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (FP_Gun_Mesh.Succeeded())
	{
		FP_Gun->SetSkeletalMesh(FP_Gun_Mesh.Object);
	}
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("AnimMontage'/Game/Mannequin/Animations/FireAnimMontage.FireAnimMontage'"));
	if (AM.Succeeded())
	{
		FireAnimation = AM.Object;
	}

	static ConstructorHelpers::FClassFinder<AMyProjectile> P(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/BP_MyProjectile.BP_MyProjectile_C'"));
	if (P.Succeeded())
	{
		ProjectileClass = P.Class;
		//UE_LOG(LogTemp, Warning, TEXT("Projectile class registeration"));
	}
	//ProjectileClass = CreateDefaultSubobject<AMyProjectile>(TEXT("PC"))->StaticClass();

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;


	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	//FPCamera->SetRelativeRotation(FRotator(0.f, 90.f, 270.f).Quaternion());
	FPCamera->SetRelativeLocation(FVector(40.f, 0.f, 60.f));
	FPCamera->SetupAttachment(RootComponent);
	FPCamera->SetActive(true);

	CharacterMeshComponent->SetupAttachment(FPCamera);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->TargetArmLength = 145.f;
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 77.f));
	CameraBoom->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
	
	
	//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	//FollowCamera->bUsePawnControlRotation = true;
	FollowCamera->SetActive(false);
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	Stat = CreateDefaultSubobject<UMyStatComponent>(TEXT("Stat"));
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFPSv1Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSv1Character::OnFire);
	PlayerInputComponent->BindAction("ToggleCameraPerspective", IE_Pressed, this, &AFPSv1Character::ToggleCameraPerspective);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSv1Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSv1Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFPSv1Character::TurnAtRate);
	//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSv1Character::AddPitchInput);
}

float AFPSv1Character::GetUpDown()
{
	return UpDown;
}

float AFPSv1Character::GetRightLeft()
{
	return RightLeft;
}

void AFPSv1Character::AddPitchInput(float Value)
{
	APawn::AddControllerPitchInput(Value);

	if (FPCamera->IsActive())
	{
		float CameraPitchValue = FPCamera->GetRelativeRotation().Pitch;
		if((Value <= 0 && CameraPitchValue <= 70.f) || (Value >= 0 && CameraPitchValue >= -70.f))
			FPCamera->AddRelativeRotation(FRotator(-Value, 0.f, 0.f));
	}
	else
	{
		//CameraBoom->AddRelativeRotation(FRotator(-Value, 0.f, 0.f));
		float CameraPitchValue = CameraBoom->GetRelativeRotation().Pitch;
		if ((Value <= 0 && CameraPitchValue <= 70.f) || (Value >= 0 && CameraPitchValue >= -70.f))
			CameraBoom->AddRelativeRotation(FRotator(-Value, 0.f, 0.f));
	}
	
}

void AFPSv1Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPSv1Character::OnFire()
{
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();

		const FRotator SpawnRotation = GetControlRotation();
		const FRotator BaseAimRotation = GetBaseAimRotation();

		
		UE_LOG(LogTemp, Warning, TEXT("CameraRotation : %s | ControlRotation : %s | BaseAimRotation : %s"), *FPCamera->GetRelativeRotation().ToString() , *SpawnRotation.ToString(), *BaseAimRotation.ToString());
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		// const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);
		const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);


		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// spawn the projectile at the muzzle
		World->SpawnActor<AMyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		DrawDebugLine(GetWorld(), SpawnLocation, SpawnLocation + SpawnRotation.Vector() * 10000.f, FColor::Red, false, 10.0f);
		//UE_LOG(LogTemp, Warning, TEXT("PlayerSpawnRotation : %s"), *SpawnRotation.ToString());
	}

	if (FireAnimation != nullptr)
	{
		UAnimInstance* AnimInstance = CharacterMeshComponent->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation);
		}
	}
}

void AFPSv1Character::ToggleCameraPerspective()
{
	if (FPCamera->IsActive())
	{
		FPCamera->SetActive(false);
		FollowCamera->SetActive(true);
	}
	else
	{
		FollowCamera->SetActive(false);
		FPCamera->SetActive(true);
	}
	return;
}

void AFPSv1Character::BeginPlay()
{
	Super::BeginPlay(); 
	
	FP_Gun->AttachToComponent(CharacterMeshComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	//FPCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("head"));
	//FPCamera->SetRelativeRotation(FRotator(0.f, 90.f, 270.f).Quaternion());

	Stat->SetStat(Name);
}

void AFPSv1Character::MoveForward(float Value)
{
	if ((Controller != nullptr) )
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(GetActorForwardVector(), Value);

		UpDown = Value;
	}
}

void AFPSv1Character::MoveRight(float Value)
{
	if ( (Controller != nullptr) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);

		RightLeft = Value;
	}
}
