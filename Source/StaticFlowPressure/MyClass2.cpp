// Fill out your copyright notice in the Description page of Project Settings.


#include "MyClass2.h"

AMyClass2::AMyClass2()
{
	TimeCounter = 0;
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterial> splineMaterialAsset(TEXT("Material'/Game/SplineMaterials/SplineMaterial_Test1.SplineMaterial_Test1'"));
	Material1 = splineMaterialAsset.Object;
	static ConstructorHelpers::FObjectFinder<UMaterial> splineMaterialAsset2(TEXT("Material'/Game/SplineMaterials/SplineMaterial_Test2.SplineMaterial_Test2'"));
	Material2 = splineMaterialAsset2.Object;

	// Static mesh
	USceneComponent* root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(root);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString("StaticMesh"));
	StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AMyClass2::BeginPlay()
{
	Super::BeginPlay();
	//GetStaticMeshComponent()->CreateDynamicMaterialInstance(0, Material1);
	StaticMeshComponent->CreateDynamicMaterialInstance(0, Material1);
}

void AMyClass2::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	TimeCounter += deltaTime;

	if (TimeCounter > 2)
	{
		//GetStaticMeshComponent()->CreateDynamicMaterialInstance(0, Material2);
		StaticMeshComponent->CreateDynamicMaterialInstance(0, Material2);
	}
}
