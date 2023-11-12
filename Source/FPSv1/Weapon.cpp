// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Engine/Texture2D.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComponent"));
	BoxComp->SetBoxExtent(FVector(1.f, 45.f, 15.f));
	BoxComp->SetCollisionProfileName(TEXT("Item"));
	BoxComp->SetSimulatePhysics(true);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnEndOverlap);
	SetRootComponent(BoxComp);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMeshComponent"));
	//SkeletalMesh->SetCollisionProfileName(TEXT("Item"));
	//SetRootComponent(SkeletalMesh);
	SkeletalMesh->SetupAttachment(RootComponent);

	SkeletalMeshOutline = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMeshOutline"));
	SkeletalMeshOutline->SetupAttachment(SkeletalMesh);
	//SkeletalMeshOutline->AttachToComponent(SkeletalMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	//check if it is correct
	//StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/UI/M_Outline.M_Outline'"));
	//SkeletalMesh->CreateDynamicMaterialInstance(1)->SetVectorParameterValue(TEXT("Color"), )

	//PopupWidget = CreateDefaultSubobject<UChildActorComponent>(TEXT("PopupWidget"));

	//static ConstructorHelpers::FClassFinder<UUserWidget> PW(TEXT("WidgetBlueprint'/Game/UI/BPW_InteractWidget.BPW_InteractWidget_C'"));
	//if (PW.Succeeded())
	//{
	//	PopupWidget->SetChildActorClass(PW.Class);
	//}

}

void AWeapon::LoadWeaponData(FName WeaponName, FString WeaponMeshPath, WeaponType WeaponType, float WeaponDamage, AmmoType WeaponAmmoType, int32 MaximumAmmoNum, FString TexturePath)
{
	const TCHAR* Path = WeaponMeshPath.GetCharArray().GetData();
	USkeletalMesh* NewMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, Path));
	if (NewMesh)
	{
		SkeletalMesh->SetSkeletalMesh(NewMesh);
		//just do it
		SkeletalMeshOutline->SetSkeletalMesh(NewMesh);
		const TCHAR* MaterialPath = L"Material'/Game/UI/M_Outline.M_Outline'";
		UMaterialInterface* material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, MaterialPath));
		int32 MaterialNum = SkeletalMeshOutline->GetNumMaterials();
		for (int i = 0; i < MaterialNum; ++i)
			SkeletalMeshOutline->SetMaterial(i, material);
		SkeletalMeshOutline->SetMaterial(0, material);
		SkeletalMeshOutline->SetVisibility(false);
		//SkeletalMeshOutline->CreateDynamicMaterialInstance(2, material);
	}

	const TCHAR* PathForTexture = TexturePath.GetCharArray().GetData();
	UTexture2D* NewTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, PathForTexture));
	if (NewTexture)
	{
		ImageTexture = NewTexture;
	}

	NameOfItem = WeaponName;
	TypeOfWeapon = WeaponType;
	Damage = WeaponDamage;
	TypeOfAmmo = WeaponAmmoType;
	MaxNumAmmo = MaximumAmmoNum;
}

void AWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("on begin overlap"));
	SkeletalMeshOutline->SetVisibility(true);
}

void AWeapon::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("on end overlap"));
	SkeletalMeshOutline->SetVisibility(false);
}