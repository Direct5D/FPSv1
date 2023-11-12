// Fill out your copyright notice in the Description page of Project Settings.


#include "CapturePlayer.h"

#include "Inventory.h"
//#include "Weapon.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/ActorComponent.h"

// Sets default values
ACapturePlayer::ACapturePlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneCaptureComp2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComp2D"));
	SceneCaptureComp2D->SetRelativeLocation(FVector(-62.f, 110.f, 15.f));
	SceneCaptureComp2D->SetRelativeRotation(FRotator(0.f, -60.f, -10.f));
	SceneCaptureComp2D->ProjectionType = ECameraProjectionMode::Orthographic;
	SceneCaptureComp2D->OrthoWidth = 100.f;
	SceneCaptureComp2D->SetupAttachment(RootComponent);
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> DefaultAnimInstance(TEXT("AnimBlueprint'/Game/ThirdPersonCPP/Blueprints/BP_CapturePlayerAnimInstance.BP_CapturePlayerAnimInstance_C'"));
	if (DefaultAnimInstance.Succeeded())
		GetMesh()->SetAnimInstanceClass(DefaultAnimInstance.Class);
	
	/*
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RenderTargetSource(TEXT("TextureRenderTarget2D'/Game/Geometry/Materials/TextureRenderTarget.TextureRenderTarget'"));
	if (RenderTargetSource.Succeeded())
	{
		SceneCaptureComp2D->TextureTarget = RenderTargetSource.Object;
	}
	*/
	
	for(int idx = 0; idx <= MAXIMUM_SLOT; idx++)
		WeaponSkMeshComps[idx] = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh" + idx));
}

// Called when the game starts or when spawned
void ACapturePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	GetMesh()->SetSkeletalMesh(OriginalCharacter->GetMesh()->SkeletalMesh);

	FName attachmentSlotName[MAXIMUM_SLOT + 1] = { "FirstMainWeaponPoint", "SecondMainWeaponPoint", "HipSocket", "GripPoint" };

	for (int idx = 0; idx <= MAXIMUM_SLOT; idx++)
		WeaponSkMeshComps[idx]->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), attachmentSlotName[idx]);
}

// Called every frame
void ACapturePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AWeapon** MyWeapons = OriginalCharacter->GetInventory()->GetWeaponInventory();

	CurrentWeaponSlot = OriginalCharacter->GetCurrentWeaponSlot();
	if (CurrentWeaponSlot == -1)
		WeaponSkMeshComps[MAXIMUM_SLOT]->SetSkeletalMesh(nullptr);

	for (int idx = 0; idx < MAXIMUM_SLOT; idx++)
	{
		if (idx == CurrentWeaponSlot)
		{
			WeaponSkMeshComps[idx]->SetSkeletalMesh(nullptr);
			WeaponSkMeshComps[MAXIMUM_SLOT]->SetSkeletalMesh(MyWeapons[idx]->GetSkeletalMeshComponent()->SkeletalMesh);
		}
		else if(MyWeapons[idx])
			WeaponSkMeshComps[idx]->SetSkeletalMesh(MyWeapons[idx]->GetSkeletalMeshComponent()->SkeletalMesh);
		else
			WeaponSkMeshComps[idx]->SetSkeletalMesh(nullptr);
	}
}

