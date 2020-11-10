// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticSensor.h"

// Sets default values
AStaticSensor::AStaticSensor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = VisualMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

	if (CubeVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(CubeVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}

// Called when the game starts or when spawned
void AStaticSensor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStaticSensor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

