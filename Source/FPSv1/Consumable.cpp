// Fill out your copyright notice in the Description page of Project Settings.

#include "Consumable.h"
#include "Components/BoxComponent.h"
#include "Engine/Texture2D.h"

AConsumable::AConsumable()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComponent"));
	BoxComp->SetBoxExtent(FVector(10.f, 15.f, 1.f));
	//BoxComp->SetRelativeScale3D();
	BoxComp->SetCollisionProfileName(TEXT("Item"));
	BoxComp->SetSimulatePhysics(true);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AConsumable::OnBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AConsumable::OnEndOverlap);
	SetRootComponent(BoxComp);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConsumableStaticMeshComponent"));
	StaticMeshOutline = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConsumableStaticMeshOutline"));
	//StaticMeshOutline->AttachToComponent(StaticMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	StaticMeshOutline->SetupAttachment(StaticMesh);
	//StaticMesh->SetCollisionProfileName(TEXT("Item"));
	//SetRootComponent(StaticMesh);
	StaticMesh->AddLocalRotation(FRotator(90.f, 0.f, 0.f));
	StaticMesh->SetupAttachment(RootComponent);
}

void AConsumable::LoadConsumableData(FName ConsumableName, FString ConsumableMeshPath, ConsumableType ConsumableType, AmmoType Ammo, int32 MaximumNum, int32 CurrentNum, float WeightOfItem, FString TexturePath)
{
	const TCHAR* Path = ConsumableMeshPath.GetCharArray().GetData();
	const TCHAR* MaterialPath = L"Material'/Game/UI/M_Outline.M_Outline'";
	UStaticMesh* NewMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, Path));
	if (NewMesh)
	{
		StaticMesh->SetStaticMesh(NewMesh);
		StaticMeshOutline->SetStaticMesh(NewMesh);
		UMaterialInterface* material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, MaterialPath));
		
		int32 MaterialNum = StaticMeshOutline->GetNumMaterials();
		for(int i=0; i<MaterialNum; ++i)
			StaticMeshOutline->SetMaterial(i, material);
		StaticMeshOutline->SetVisibility(false);
		//StaticMeshOutline->CreateDynamicMaterialInstance(0, material);
	}

	const TCHAR* PathForTexture = TexturePath.GetCharArray().GetData();
	UTexture2D* NewTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, PathForTexture));
	if (NewTexture)
	{
		ImageTexture = NewTexture;
	}

	NameOfItem = ConsumableName;
	TypeOfItem = ConsumableType;
	TypeOfAmmo = Ammo;
	MaxAmount = MaximumNum;
	CurrentAmount = CurrentNum;
	UnitWeight = WeightOfItem;
}

void AConsumable::SetCurrentNum(int32 NewNum)
{
	CurrentAmount = NewNum;
	OnChangedItem.Broadcast();
}

void AConsumable::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("on begin overlap"));
	StaticMeshOutline->SetVisibility(true);
}

void AConsumable::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("on end overlap"));
	StaticMeshOutline->SetVisibility(false);

}