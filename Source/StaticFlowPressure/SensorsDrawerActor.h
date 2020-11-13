// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SensorsDrawerActor.generated.h"

UCLASS()
class STATICFLOWPRESSURE_API ASensorsDrawerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASensorsDrawerActor();

	UPROPERTY(EditAnywhere);
		UParticleSystemComponent* ParticleSystem;

	UPROPERTY(EditAnywhere, Category = "Calculation")
		FVector Resolution;	// Масштаб по осям

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	bool _isTickStarted = false;
	bool _isEmitterMoved = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
