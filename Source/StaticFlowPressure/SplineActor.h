// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
//#include "Components/SplineMeshComponent.h"
#include "GameFramework/Actor.h"

//#include "Engine/DataTable.h"

#include "SplineActor.generated.h"

UCLASS()
class STATICFLOWPRESSURE_API ASplineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineActor();

	void OnConstruction(const FTransform& Transform) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UStaticMesh* Asset;

	UPROPERTY(VisibleAnywhere, Category = "Spline")
		USplineComponent* SplineComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		UStaticMeshComponent* MeshComponent;

	int on = 0;
	int i = 0;
};
