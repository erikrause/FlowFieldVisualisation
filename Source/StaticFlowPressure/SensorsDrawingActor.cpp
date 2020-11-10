// Fill out your copyright notice in the Description page of Project Settings.


#include "SensorsDrawingActor.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASensorsDrawingActor::ASensorsDrawingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//static ConstructorHelpers::FObjectFinder<UMaterial> particleSystemAsset(TEXT("/Game/BunnyParticleSystem.BunnyParticleSystem"));

	//UParticleSystemComponent* particleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(*FString("Particle" + FString::FromInt(1)));

	//particleSystemComponent->setpa

}

// Called when the game starts or when spawned
void ASensorsDrawingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASensorsDrawingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

