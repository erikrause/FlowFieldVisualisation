// Fill out your copyright notice in the Description page of Project Settings.
PRAGMA_DISABLE_OPTIMIZATION

#include "SensorsDrawerActor.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASensorsDrawerActor::ASensorsDrawerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	Resolution = FVector(20, 20, 20);

	//static ConstructorHelpers::FObjectFinder<UMaterial> particleSystemAsset(TEXT("/Game/BunnyParticleSystem.BunnyParticleSystem"));

	//UParticleSystemComponent* particleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(*FString("Particle" + FString::FromInt(1)));

	//particleSystemComponent->setpa

}

// Called when the game starts or when spawned
void ASensorsDrawerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASensorsDrawerActor::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ASensorsDrawerActor, ParticleSystem))
	{
		ParticleSystem->AttachTo(RootComponent);
	}
}

// Called every frame
void ASensorsDrawerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!_isEmitterMoved) 
	{
		if (_isTickStarted)
		{
			//auto location = ParticleSystem->GetRelativeLocation();
			FVector location = FVector(Resolution.X * Resolution.Y * Resolution.Z, 0, 0);
			ParticleSystem->SetRelativeLocation(location);

			_isEmitterMoved = true;
		}
		else
		{
			_isTickStarted = true;
		}
	}
}

