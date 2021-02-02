// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MyClass2.generated.h"

/**
 * 
 */
UCLASS()
class STATICFLOWPRESSURE_API AMyClass2 : public AActor
{
	GENERATED_BODY()
public:
	AMyClass2();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	float TimeCounter;
	UMaterial* Material1;
	UMaterial* Material2;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
};