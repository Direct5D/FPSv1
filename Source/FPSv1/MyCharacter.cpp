// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "MyProjectile.h"
#include "MyCharacterStatWidget.h"
#include "NotificationWidget.h"
#include "MyStatComponent.h"
#include "Inventory.h"
#include "Item.h"
#include "InventoryWidget.h"

#include "Animation/AnimInstance.h"

#include "Camera/CameraComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"

#include "DrawDebugHelpers.h"

#include "Engine/World.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Internationalization/Text.h"


AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->InitCapsuleSize(34.f, 92.f);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharSkeletalMesh(TEXT("SkeletalMesh'/Game/Mixamo/Y_Bot.Y_Bot'"));
	if (CharSkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharSkeletalMesh.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -92.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, 270.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> DefaultAnimInstance(TEXT("AnimBlueprint'/Game/ThirdPersonCPP/Blueprints/BP_MyAnimInstance.BP_MyAnimInstance_C'"));
	if (DefaultAnimInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(DefaultAnimInstance.Class);
	}

	GetCharacterMovement()->bOrientRotationToMovement = false; 
	GetCharacterMovement()->JumpZVelocity = 380.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	CurrentPerspectiveMode = PerspectiveMode::FirstPerson;

	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	FPCamera->SetRelativeLocation(FVector(40.f, 0.f, 55.f));
	FPCamera->SetupAttachment(RootComponent);
	FPCamera->SetActive(true);
	FPCamera->bUsePawnControlRotation = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 95.f));
	CameraBoom->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	CameraBoom->SocketOffset = FVector(0.f, 30.f, 0.f);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;

	TPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPCamera"));
	TPCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TPCamera->SetActive(false);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PunchingMontageObject(TEXT("AnimMontage'/Game/Mixamo/Animations/MeleeAnim/Punching_Montage.Punching_Montage'"));
	if (PunchingMontageObject.Succeeded())
	{
		PunchingMontage = PunchingMontageObject.Object;
	}
	CurrentAttackMontage = PunchingMontage;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FiringMontageObject(TEXT("AnimMontage'/Game/Mixamo/Animations/RifleAnim/Fire_Rifle_Ironsights_Montage.Fire_Rifle_Ironsights_Montage'"));
	if (PunchingMontageObject.Succeeded())
	{
		FiringMontage = FiringMontageObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SecondaryFiringMontageObject(TEXT("AnimMontage'/Game/Mixamo/Animations/GunAnim/SecondaryFiringMontage.SecondaryFiringMontage'"));
	if (SecondaryFiringMontageObject.Succeeded())
	{
		SecondaryFiringMontage = SecondaryFiringMontageObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadingMontageObject(TEXT("AnimMontage'/Game/Mixamo/Animations/RifleAnim/ReloadingMontage.ReloadingMontage'"));
	if (ReloadingMontageObject.Succeeded())
	{
		ReloadingMontage = ReloadingMontageObject.Object;
	}
	

	PlayerInventory = CreateDefaultSubobject<UInventory>(TEXT("PlayerInventory"));
	
	static ConstructorHelpers::FClassFinder<UInventoryWidget> IW(TEXT("WidgetBlueprint'/Game/UI/BPW_Inventory.BPW_Inventory_C'"));
	if (IW.Succeeded())
	{
		InventoryWidgetClass = IW.Class;
	}
	InventoryWidget = Cast<UInventoryWidget>(CreateWidget(GetWorld(), InventoryWidgetClass));
	
	
	ScanItemComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ScanItemComponent"));
	ScanItemComponent->SetupAttachment(GetRootComponent());
	//ScanItemComponent->SetRelativeLocation(GetMesh()->GetRelativeLocation());
	ScanItemComponent->SetRelativeLocation(FVector(0.f, 0.f, -92.f));
	ScanItemComponent->SetSphereRadius(200.f);
	ScanItemComponent->SetCollisionProfileName(TEXT("Interact"));	//
	//ScanItemComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);

	StatComponent = CreateDefaultSubobject<UMyStatComponent>(TEXT("PlayerStat"));

	static ConstructorHelpers::FClassFinder<UMyCharacterStatWidget> CSW(TEXT("WidgetBlueprint'/Game/UI/BPW_Stat.BPW_Stat_C'"));
	if (CSW.Succeeded())
	{
		StatWidgetClass = CSW.Class;
	}
	StatWidget = Cast<UMyCharacterStatWidget>(CreateWidget(GetWorld(), StatWidgetClass));

	static ConstructorHelpers::FClassFinder<UNotificationWidget> NW(TEXT("WidgetBlueprint'/Game/UI/BPW_Notification.BPW_Notification_C'"));
	if (NW.Succeeded())
	{
		NotiWidgetClass = NW.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> ITA(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/BPW_TestActor.BPW_TestActor_C'"));
	if (ITA.Succeeded())
	{
		InteractTestActorClass = ITA.Class;
	}
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerInventory->FuncDeleSingleOneParamWhenGetWeapon.BindUFunction(this, FName("GetWeapon"));
	PlayerInventory->FuncDeleSingleOneParamWhenDropWeapon.BindUFunction(this, FName("DropWeapon"));
	PlayerInventory->FuncDeleSingleOneParamWhenSwitchWeapon.BindUFunction(this, FName("SwitchWeapon"));
	//PlayerInventory->FuncDeleSingleOneParamWhenDropWeapon.BindUFunction(this, FName("SwitchWeapon"));

	InventoryWidget->BindPlayerCharacter(this);

	//WeaponListWidget->BindWithWeaponData(PlayerInventory->GetWeaponInventory());
	//InventoryWidget->BindWeaponWidget(WeaponListWidget);
	StatComponent->SetStat(PlayerName);
	StatComponent->OnDeath.AddUObject(this, &AMyCharacter::Death);
	if (StatWidget)
	{
		StatWidget->BindWithStat(StatComponent);
		StatWidget->AddToViewport();
		StatWidget->NotifyWidgetClass = NotiWidgetClass;
	}

	if (InventoryWidget)
	{
		InventoryWidget->AddToViewport();
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	//BaseActorRotation = GetActorRotation();
	UpdatePitchLimits(-90.f, 90.f);

	if (CurveFloat_CameraArmLength && CurveVector_CameraSocketOffset)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		CurveTimeline.AddInterpFloat(CurveFloat_CameraArmLength, TimelineProgress);

		FOnTimelineVector TimelineProgressVector;
		TimelineProgressVector.BindUFunction(this, FName("TimelineProgressVector"));
		CurveTimeline.AddInterpVector(CurveVector_CameraSocketOffset, TimelineProgressVector);

		CurveTimeline.SetLooping(false);
	}

	UE_LOG(LogTemp, Warning, TEXT("MyCharacter animation instance %s"), *GetMesh()->GetAnimInstance()->GetName());
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurveTimeline.TickTimeline(DeltaTime);

	FRotator AimRotator(AimPitch, AimYaw, 0.f);
	FRotator Delta = GetControlRotation() - GetActorRotation();

	AimPitch = UKismetMathLibrary::ClampAngle(Delta.Pitch, -90.f, 90.f);
	AimYaw = UKismetMathLibrary::ClampAngle(Delta.Yaw, -90.f, 90.f);

	if (CurrentWeaponCanNull)
		WeaponLeftHandTarget = CurrentWeaponCanNull->GetSkeletalMeshComponent()->GetSocketLocation(TEXT("WeaponLeftHandTarget"));


	FVector SpawnLocation, EndLocation;
	if (FPCamera->IsActive())
	{
		SpawnLocation = FPCamera->GetComponentLocation();
		EndLocation = SpawnLocation + FPCamera->GetForwardVector() * 250.f;
	}
	else
	{
		SpawnLocation = TPCamera->GetComponentLocation() + TPCamera->GetForwardVector() * CameraBoom->TargetArmLength;
		EndLocation = SpawnLocation + TPCamera->GetForwardVector() * 250.f;
	}

	/*
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByProfile(Hit, SpawnLocation, EndLocation, FName("Item")) == true)
	{
		AItem* TargetItem = Cast<AItem>(Hit.GetActor());
		if (TargetItem && TargetItem->GetClass()->ImplementsInterface(UInteract::StaticClass()))
		{
			TargetItem->Detect_Implemenetation();
		}
	}
	*/
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind Action
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::MyJump);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AMyCharacter::Fire);
	PlayerInputComponent->BindAction(TEXT("ToggleCameraPerspective"), EInputEvent::IE_Pressed, this, &AMyCharacter::SetPerspectiveMode);
	PlayerInputComponent->BindAction(TEXT("Scope"), EInputEvent::IE_Pressed, this, &AMyCharacter::Scope);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &AMyCharacter::MyCrouch);
	PlayerInputComponent->BindAction(TEXT("Prone"), EInputEvent::IE_Pressed, this, &AMyCharacter::MyProne);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMyCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMyCharacter::UnSprint);
	PlayerInputComponent->BindAction(TEXT("ShoulderHold"), EInputEvent::IE_Pressed, this, &AMyCharacter::ShoulderHoldOn);
	PlayerInputComponent->BindAction(TEXT("ShoulderHold"), EInputEvent::IE_Released, this, &AMyCharacter::ShoulderHoldOff);
	PlayerInputComponent->BindAction<FInputSwitchWeaponDelegate>(TEXT("Weapon1"), EInputEvent::IE_Pressed, this, &AMyCharacter::SwitchWeapon, FIRST_SLOT);
	PlayerInputComponent->BindAction<FInputSwitchWeaponDelegate>(TEXT("Weapon2"), EInputEvent::IE_Pressed, this, &AMyCharacter::SwitchWeapon, SECOND_SLOT);
	PlayerInputComponent->BindAction<FInputSwitchWeaponDelegate>(TEXT("Holster"), EInputEvent::IE_Pressed, this, &AMyCharacter::SwitchWeapon, NONE_WEAPON);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AMyCharacter::MyReload);
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AMyCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("OpenInventory"), EInputEvent::IE_Pressed, this, &AMyCharacter::OpenInventory);
	PlayerInputComponent->BindAction(TEXT("ChangeShotMode"), EInputEvent::IE_Pressed, this, &AMyCharacter::ChangeShotMode);

	//Bind Axis
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::LeftRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMyCharacter::AddYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyCharacter::AddPitchInput);
}

void AMyCharacter::TimelineProgress(float Value)
{
	float NewArmLength = FMath::Lerp(250.f, 100.f, Value);
	CameraBoom->TargetArmLength = NewArmLength;
}

void AMyCharacter::TimelineProgressVector(FVector Value)
{
	FVector NewSocketOffset = FMath::Lerp(FVector(0.f, 25.f, 25.f), FVector(0.f, 35.f, 0.f), Value);
	CameraBoom->SocketOffset = NewSocketOffset;
}

void AMyCharacter::SwitchWeapon(const int32 WeaponIndex)
{
	FName attachmentSlotName[MAXIMUM_SLOT + 1] = { "FirstMainWeaponPoint", "SecondMainWeaponPoint", "HipSocket", "GripPoint"};

	// When switching happens within Inventory widget
	if (WeaponIndex == MAXIMUM_SLOT)
	{
		AWeapon** CurrentWeaponInventory = PlayerInventory->GetWeaponInventory();
		for (int32 i = 0; i < MAXIMUM_SLOT; i++)
		{
			if (CurrentWeaponInventory[i])
			{
				CurrentWeaponInventory[i]->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), attachmentSlotName[i]);
			}
			if (i == CurrentWeaponSlot)
			{
				CurrentWeaponCanNull = CurrentWeaponInventory[i];
				
				if (CurrentWeaponCanNull)
				{
					CurrentWeaponSlot = i;
					StatComponent->SetNewWeapon(CurrentWeaponCanNull->GetMaximumAmmo(), CurrentWeaponCanNull->GetCurrentAmmo());
					CurrentWeaponInventory[i]->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), attachmentSlotName[MAXIMUM_SLOT]);
					FuncDeleSingleWeapon.ExecuteIfBound();
				}
				else
				{
					CurrentWeaponSlot = NONE_WEAPON;
					StatComponent->SetNewWeapon(0, 0);
					FuncDeleSingleNoWeapon.ExecuteIfBound();
				}	
				CurrentAttackMontage = GetCurrentAttachMontage(CurrentWeaponSlot);
			}
		}
		return;
	}

	// weapon holstering (player press X)
	if (WeaponIndex == NONE_WEAPON)
	{
		if (CurrentWeaponSlot == NONE_WEAPON) return;
		
		AWeapon** CurrentWeaponInventory = PlayerInventory->GetWeaponInventory();
		AWeapon* CurrentWeapon = CurrentWeaponInventory[CurrentWeaponSlot];
		if (CurrentWeapon != nullptr)
		{
			switch (CurrentWeaponSlot)
			{
			case FIRST_SLOT:
				CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), attachmentSlotName[FIRST_SLOT]);
				break;
			case SECOND_SLOT:
				CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), attachmentSlotName[SECOND_SLOT]);
				break;
			case THIRD_SLOT:
				CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), attachmentSlotName[THIRD_SLOT]);
				break;
			}
		}
		CurrentWeaponCanNull = nullptr;
		CurrentAttackMontage = GetCurrentAttachMontage(NONE_WEAPON);
		CurrentWeaponSlot = NONE_WEAPON;
		StatComponent->SetNewWeapon(0, 0);

		// trigger weapon ui delegate
		FuncDeleSingleNoWeapon.ExecuteIfBound();

		return;
	}

	int TargetWeaponSlot = WeaponIndex;
	AWeapon** CurrentWeaponInventory = PlayerInventory->GetWeaponInventory();
	AWeapon* TargetWeapon = CurrentWeaponInventory[TargetWeaponSlot];
	if (TargetWeapon == nullptr || CurrentWeaponSlot == TargetWeaponSlot) return;
	
	AWeapon* CurrentWeapon = CurrentWeaponInventory[CurrentWeaponSlot];
	if (CurrentWeapon != nullptr)
	{
		switch (CurrentWeaponSlot)
		{
		case NONE_WEAPON:
			break;
		case FIRST_SLOT:
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), attachmentSlotName[FIRST_SLOT]);
			break;
		case SECOND_SLOT:
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), attachmentSlotName[SECOND_SLOT]);
			break;
		case THIRD_SLOT:
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), attachmentSlotName[THIRD_SLOT]);
			break;
		}
	}
	TargetWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), attachmentSlotName[MAXIMUM_SLOT]);
	CurrentWeaponCanNull = TargetWeapon;
	CurrentAttackMontage = GetCurrentAttachMontage(TargetWeaponSlot);
	CurrentWeaponSlot = TargetWeaponSlot;
	FuncDeleSingleWeapon.ExecuteIfBound();

	StatComponent->SetNewWeapon(TargetWeapon->GetMaximumAmmo(), TargetWeapon->GetCurrentAmmo());
}

void AMyCharacter::GetWeapon(int32 WeaponSlotNum)
{
	if (CurrentWeaponSlot == NONE_WEAPON)
	{
		SwitchWeapon(WeaponSlotNum);
	}
	else
	{
		AWeapon* CurrentWeapon = PlayerInventory->GetWeaponInventory()[WeaponSlotNum];
		switch (WeaponSlotNum)
		{
		case FIRST_SLOT:
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("FirstMainWeaponPoint"));
			break;
		case SECOND_SLOT:
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("SecondMainWeaponPoint"));
			break;
		case THIRD_SLOT:
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("HipSocket"));
			break;
		}
	}
}

void AMyCharacter::DropWeapon(int32 TargetWeaponSlotNum)
{
	AWeapon* TargetWeapon = PlayerInventory->GetWeaponInventory()[TargetWeaponSlotNum];
	if (TargetWeaponSlotNum == CurrentWeaponSlot)
	{
		CurrentWeaponCanNull = nullptr;
		CurrentAttackMontage = GetCurrentAttachMontage(NONE_WEAPON);
		CurrentWeaponSlot = NONE_WEAPON;
		StatComponent->SetNewWeapon(0, 0);
		
	}
	TargetWeapon->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepRelative, false));
	return;
}

void AMyCharacter::DamagedFromPlayer(FName HitPlayerName, float HitDamage)
{
	float NewHp = StatComponent->GetCurrentHp() - HitDamage * 10;
	StatComponent->SetNewHp(NewHp);
}

UAnimMontage* AMyCharacter::GetCurrentAttachMontage(int WeaponIdx) const
{
	UAnimMontage* TargetAttackAnimMontage;
	switch (WeaponIdx)
	{
	case NONE_WEAPON:
		TargetAttackAnimMontage = PunchingMontage;
		break;
	case FIRST_SLOT:
	case SECOND_SLOT:
		TargetAttackAnimMontage = FiringMontage;
		break;
	case THIRD_SLOT:
		TargetAttackAnimMontage = SecondaryFiringMontage;
		break;
	default:
		TargetAttackAnimMontage = PunchingMontage;
		break;
	}
	return TargetAttackAnimMontage;
}

void AMyCharacter::Death()
{
	IsDying = true;
	// restart or.. destroy or.. endplay ?
}

void AMyCharacter::Fire()
{	

	if (GetWorld()->GetTimerManager().IsTimerActive(myTimerHandle) == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("other action playing.."));
		//notify
		FuncDeleNotifyOneParam.ExecuteIfBound(FText::FromString("other action playing.."));
		return;
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	//SyncControlActorOrient();

	//FRotator AimRotator(AimPitch, AimYaw, 0.f);
	//FRotator Delta = GetControlRotation() - GetActorRotation();

	//AimPitch = UKismetMathLibrary::ClampAngle(Delta.Pitch, -90.f, 90.f);
	//AimYaw = UKismetMathLibrary::ClampAngle(Delta.Yaw, -90.f, 90.f);

	UE_LOG(LogTemp, Warning, TEXT("Current Attack Montage play length = %f"), CurrentAttackMontage->GetPlayLength());
	// throw punch
	if (CurrentWeaponSlot == NONE_WEAPON)
	{
		if (AnimInstance && AnimInstance->IsAnyMontagePlaying() == false)
		{
			AnimInstance->Montage_Play(CurrentAttackMontage);
		}

		DisableInput(Cast<APlayerController>(GetController()));
		
		GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				EnableInput(Cast<APlayerController>(GetController()));

				GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);
			}), CurrentAttackMontage->GetPlayLength(), false);

		// detect and if detection success, trigger delegate DamageFromPlayer

		FHitResult Out;
		FVector ActorLoc = GetActorLocation();
		ActorLoc.Z = 250.f;
		ActorLoc += GetActorForwardVector() * 60.f;
		
		TArray<AActor*> actorsToIgnore;
		actorsToIgnore.Add(this);

		if (UKismetSystemLibrary::CapsuleTraceSingleByProfile(GetWorld(), ActorLoc, ActorLoc, 40.f, 40.f, FName("Punch"),
			true, actorsToIgnore, EDrawDebugTrace::ForDuration, Out, true) == true)
		{
			if (Out.GetActor()->IsA<AMyCharacter>() == true)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *Out.BoneName.ToString());
				AMyCharacter* hitCharacter = Cast<AMyCharacter>(Out.GetActor());
				if (hitCharacter != nullptr)
					hitCharacter->DamagedFromPlayer(GetStatComponent()->GetName(), GetPlayerDamage());
			}
		}
		return;
	}

	// fire
	if (CurrentWeaponCanNull && CurrentWeaponCanNull->GetCurrentAmmo() == 0)
	{
		//notify
		FuncDeleNotifyOneParam.ExecuteIfBound(FText::FromString("no ammo"));
		return;
	}

	UWorld* const World = GetWorld();

	//const FRotator SpawnRotation = GetControlRotation();
	const FRotator BaseAimRotation = GetBaseAimRotation();

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	ActorSpawnParams.Owner = this;

	const FVector MuzzleLocation = CurrentWeaponCanNull->GetSkeletalMeshComponent()->GetSocketLocation(TEXT("Muzzle"));
	FRotator GunForwardRotation = CurrentWeaponCanNull->GetSkeletalMeshComponent()->GetSocketRotation(TEXT("Muzzle"));
	//World->SpawnActor<AMyProjectile>(AMyProjectile::StaticClass(), MuzzleLocation, BaseAimRotation, ActorSpawnParams);
	World->SpawnActor<AMyProjectile>(AMyProjectile::StaticClass(), MuzzleLocation, GunForwardRotation, ActorSpawnParams);

	DrawDebugLine(World, MuzzleLocation, MuzzleLocation + BaseAimRotation.Vector() * 5000.f, FColor::Emerald, true, 5.f, 0, 1);


	//FVector GunForwardRotation = CurrentWeaponCanNull->GetActorForwardVector();
	
	DrawDebugLine(World, MuzzleLocation, MuzzleLocation + GunForwardRotation.Vector() * 5000.f, FColor::Purple, true, 5.f, 0, 1);
	//DrawDebugPoint(World, MuzzleLocation, 10.f, FColor::Green, false, 1.f);
	CurrentWeaponCanNull->SetConsumeAAmmo();
	StatComponent->SetCurrentAmmo(CurrentWeaponCanNull->GetCurrentAmmo());

	if (AnimInstance && AnimInstance->IsAnyMontagePlaying() == false)
		AnimInstance->Montage_Play(CurrentAttackMontage);



	//DisableInput(Cast<APlayerController>(GetController()));

	//int32 boneIdx = GetMesh()->GetBoneIndex(TEXT("Spine2"));
	//InitialTransform = GetMesh()->GetBoneTransform(boneIdx);
	//InitialTransform = GetMesh()->GetBoneTransformByName(BoneName, EBoneSpaces::World);

	// Calculate final transform (recoil)
	//FinalTransform = InitialTransform;
	//FinalTransform.AddToTranslation(FVector(0.1f, 0.1f,1.f));

	//FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		//EnableInput(Cast<APlayerController>(GetController()));
		GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);
		/*LerpAlpha += 0.1f;
		if (LerpAlpha > 1.f)
		{
			GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);
			LerpAlpha = 0;
			return;
		}*/
		//FTransform CurrentTransform = FMath::Lerp(InitialTransform, FinalTransform, LerpAlpha);
		//GetMesh()->TransformToBoneSpace
		// Apply the new transform to the bone
		//GetMesh()->SetBoneTransformByName(BoneName, CurrentTransform, EBoneSpaces::World);
		//GetMesh()->bonetrans
	}), 0.1f, false);
}

void AMyCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	// when sprinting velocity size is around 300.f
	// UE_LOG(LogTemp, Warning, TEXT("Character Velocity Size: %f"), GetVelocity().Size()); 
}

void AMyCharacter::UnSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void AMyCharacter::MyJump()
{
	if (CanSwitchingPose)
	{
		// if character is crouching or proning then just stand up
		if(IsCrouching || IsProning)
		{
			IsCrouching = IsProning = false;
		}
		else
		{
			ACharacter::Jump();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can not play other pose"));
		//notify
		FuncDeleNotifyOneParam.ExecuteIfBound(FText::FromString("other action playing.."));
	}
}

void AMyCharacter::MyCrouch()
{
	if (CanSwitchingPose)
	{
		if (IsCrouching)
		{
			IsCrouching = false;
		}
		else
		{
			IsProning = false;	// Anyway IsProning is False.
			IsCrouching = true;
			ACharacter::Crouch();
		}
		// Even the key input is Jump(SpaceBar), when CanSwitchingPose is true, Jump is end.
		// Because While jumping, the flag turns off right after JumpEnd animation ended.
		
		//capsuel, box component resize
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can not play other pose"));
		//notify
		FuncDeleNotifyOneParam.ExecuteIfBound(FText::FromString("other action playing.."));
	}
}

void AMyCharacter::MyProne()
{
	if (CanSwitchingPose)
	{
		//capsule, box component resize
		if (IsProning)
		{
			IsProning = false;
		}
		else
		{
			IsCrouching = false;
			IsProning = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can not play other pose"));
		//notify
		FuncDeleNotifyOneParam.ExecuteIfBound(FText::FromString("Can not play other pose"));
	}

}

void AMyCharacter::Scope()
{
	if (IsScopeMode)
	{

	}
	else
	{
		
	}
}

void AMyCharacter::ShoulderHoldOn()
{
	CurveTimeline.PlayFromStart();
}

void AMyCharacter::ShoulderHoldOff()
{
	CurveTimeline.Reverse();
}

void AMyCharacter::MyReload()
{
	if (PlayerInventory && CurrentWeaponCanNull)
	{
		int32 DeltaAmmo = CurrentWeaponCanNull->GetDeltaAmmo();
		if (DeltaAmmo == 0)
		{
			//full
			return;
		}

		AConsumable* ammo = PlayerInventory->FindAmmoCanNull(CurrentWeaponCanNull->GetType());
		if (ammo != nullptr)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && AnimInstance->IsAnyMontagePlaying() == false)
			{
				AnimInstance->Montage_Play(ReloadingMontage);

				//DisableInput(Cast<APlayerController>(GetController()));
				//FTimerHandle myTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
					{
						//EnableInput(Cast<APlayerController>(GetController()));

						GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);
					}), ReloadingMontage->GetPlayLength(), false);
			}

			int32 RemainingAmmoNum = ammo->GetCurrentNum();
			int32 RefillAmmoNum = 0;
			if (RELOAD_AMOUNT > DeltaAmmo || RELOAD_AMOUNT > RemainingAmmoNum)
			{
				if (DeltaAmmo > RemainingAmmoNum)
				{
					RefillAmmoNum = RemainingAmmoNum;
				}
				else
				{
					RefillAmmoNum = DeltaAmmo;
				}
			}
			else
			{
				RefillAmmoNum = RELOAD_AMOUNT;
			}

			ammo->SetCurrentNum(RemainingAmmoNum - RefillAmmoNum);
			CurrentWeaponCanNull->ReloadAmmo(RefillAmmoNum);
			StatComponent->SetCurrentAmmo(CurrentWeaponCanNull->GetCurrentAmmo());

			if (ammo->GetCurrentNum() == 0)
			{
				if (PlayerInventory->RemoveFromInventory(ammo))
				{
					ammo->Destroy();
				}
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString(TEXT("no Ammo")));
			// notify
			FuncDeleNotifyOneParam.ExecuteIfBound(FText::FromString(TEXT("no Ammo")));
		}
	}
	return;
}

void AMyCharacter::Interact()
{
	FVector SpawnLocation, EndLocation;
	if (FPCamera->IsActive())
	{
		SpawnLocation = FPCamera->GetComponentLocation();
		EndLocation = SpawnLocation + FPCamera->GetForwardVector() * 250.f;
	}
	else
	{
		SpawnLocation = TPCamera->GetComponentLocation() + TPCamera->GetForwardVector() * CameraBoom->TargetArmLength;
		EndLocation = SpawnLocation + TPCamera->GetForwardVector() * 250.f;
	}

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByProfile(Hit, SpawnLocation, EndLocation, FName("Item")) == true)
	{
		AItem* TargetItem = Cast<AItem>(Hit.GetActor());
		if (TargetItem && TargetItem->GetClass()->ImplementsInterface(UInteract::StaticClass()))
			TargetItem->PickUp_Implementation();
	}

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(ActorsToIgnore);
	FHitResult Hit2;
	if (GetWorld()->LineTraceSingleByChannel(Hit2, SpawnLocation, EndLocation, ECC_Visibility, QueryParams) == true)
	{
		
		AActor* TargetActor = Hit2.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *TargetActor->GetName());
		if (TargetActor && TargetActor->GetClass()->ImplementsInterface(UInteract::StaticClass()))
			IInteract::Execute_Interact_Implementation(TargetActor);
	}
	//if (GetWorld()->LineTraceSingleByProfile(Hit2, SpawnLocation, EndLocation, FName("Visibility")) == true)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("a"));
	//	AActor* TargetActor = Hit2.GetActor();
	//	if (TargetActor && TargetActor->GetClass()->ImplementsInterface(UInteract::StaticClass()))
	//		IInteract::Execute_Interact_Implementation(TargetActor);
	//}
}

void AMyCharacter::OpenInventory()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	
	if (InventoryWidget->GetVisibility() == ESlateVisibility::Collapsed)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameAndUI I;
		I.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(I);
		PlayerController->SetShowMouseCursor(true);
	}
	else if(InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->SetShowMouseCursor(false);
	}
}

void AMyCharacter::ChangeShotMode()
{
	if (CurrentShotMode == ShotMode::Single)
	{
		CurrentShotMode = ShotMode::Continuous;
		//notify
		FuncDeleNotifyOneParam.ExecuteIfBound(FText::FromString("Shot mode : Continuous"));
	}
	else
	{
		CurrentShotMode = ShotMode::Single;
		//notify
		FuncDeleNotifyOneParam.ExecuteIfBound(FText::FromString("Shot mode : Single"));
	}
}

void AMyCharacter::UpDown(float Value)
{
	if (Value != 0.f)
	{
		SyncControlActorOrient();
	}

	AddMovementInput(GetActorForwardVector(), Value);
	UpDownValue = Value;
}

void AMyCharacter::LeftRight(float Value)
{
	if (Value != 0.f)
	{
		SyncControlActorOrient();
	}
	
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), Value);
	LeftRightValue = Value;
}

void AMyCharacter::AddYawInput(float Value)
{
	APawn::AddControllerYawInput(Value);

	if (AimYaw == 90.f)
	{
		
		IsTurningRight = true;
		IsTurningLeft = false;
		
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	if (AimYaw == -90.f)
	{
		
		IsTurningRight = false;
		IsTurningLeft = true;

		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	if (AimYaw == 0.f)
	{
		IsTurningRight = false;
		IsTurningLeft = false;

		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void AMyCharacter::AddPitchInput(float Value)
{
	APawn::AddControllerPitchInput(Value);
}

void AMyCharacter::SyncControlActorOrient()
{
	FRotator NewActorRotation(GetActorRotation());
	NewActorRotation.Yaw = GetControlRotation().Yaw;
	APawn::SetActorRotation(NewActorRotation);
	//BaseActorRotation = NewActorRotation;

	IsTurningRight = false;
	IsTurningLeft = false;
}

void AMyCharacter::SetPerspectiveMode()
{
	switch (CurrentPerspectiveMode)
	{
	case PerspectiveMode::FirstPerson:
		GetFPCamera()->SetActive(false);
		GetTPCamera()->SetActive(true);
		CurrentPerspectiveMode = PerspectiveMode::ThridPerson;
		break;

	case PerspectiveMode::ThridPerson:
		GetTPCamera()->SetActive(false);
		GetFPCamera()->SetActive(true);
		CurrentPerspectiveMode = PerspectiveMode::FirstPerson;
		break;

	default:
		check(false);	// not reach here
		break;
	}
}

void AMyCharacter::UpdatePitchLimits(float NewMinPitch, float NewMaxPitch)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->PlayerCameraManager->ViewPitchMin = NewMinPitch;
		PlayerController->PlayerCameraManager->ViewPitchMax = NewMaxPitch;
	}
}

